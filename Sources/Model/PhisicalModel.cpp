#include "Headers/Model/PhisicalModel.hpp"

#include "Headers/Model/Gauss.hpp"
/*---------------------------------------------------------------------------*/

PhisicalModel::PhisicalModel( const std::string& _filePath )
	:	m_inputFile( makeFileImpl( _filePath ) )
	,	m_outputScatteringFile( makeFileImpl( ScatteringFilePath, FileImpl::out ) )
	,	m_outputGraphicsFile( makeFileImpl( ChartFilePath, FileImpl::out ) )
{
	if ( !m_inputFile->is_open() )
		throw std::runtime_error(" File with input data non opened");
		 
} // PhisicalModel::PhisicalModel

/*---------------------------------------------------------------------------*/

PhisicalModel::~PhisicalModel()
{
	m_inputFile->close();
	m_outputScatteringFile->close();
	m_outputGraphicsFile->close();

} // PhisicalModel::~PhisicalModel

/*---------------------------------------------------------------------------*/

void 
PhisicalModel::calculatePhisicalModel()
{
	using Vector = Gauss::Vector;
	using VectorInt = std::vector < int >;

	size_t N, M, L, Itr;            // ����������� ��������

	double resonanceFreq,   // ����������� �������
		radiusBubble,		// ������ ��������
		radiusNorm,			// ������������� ������ ��������
		radiusBubbleMin,    // R_min < radiusBubble
		radiusBubbleMax,    // radiusBubble > R_max
		h,				 // ��������� �������
		eta,                // teta > 0  // �� �������, �������� ���� ����������
		deltaRadius,				 // ������ ���������� �� ������� - �� �������, �������� ���� ����������
		dd;				 // �� �������, �������� ���� ����������

	double alpha, err;   // // �� �������, �������� ���� ����������

	*m_inputFile >> N;
	*m_inputFile >> M;
	*m_inputFile >> L;
	*m_inputFile >> radiusBubbleMin >> radiusBubbleMax;
	*m_inputFile >> h;
	*m_inputFile >> eta;
	*m_inputFile >> Itr;
	*m_inputFile >> err;


	Vector F_exp( L );  // ������ ������ ������������
	Vector Aph( Itr );  // ������ ��� ������ �����, ����� ����� ������ �� �������������� ��������
	Vector delta_al( Itr );  // ������ ��������� ������


	Matrix A( M , M );  // ������� ��� ����
	Vector B( M );  // ������ ������ �����
	Vector mv_f( L );  // ��, ��� �� ������ ���������� � �������
	
	Matrix Umn( M + 1, N + 1 );  // ������� �������� ������� 
	Matrix Imn( M + 1, N + 1 );  // ������� ��������� �� �.� 
	Matrix Kmn_fr( L, N + 1 );  // ������� ���� ��������������� �������
	Vector Lm_r( M + 1 );  // ��������
	Matrix Im_N( L, M + 1 );  // ������� ��������������

		/*  ������� ��� �������� ���������� ��������� */
	Vector Nm( N );  
	Vector Nm_s( N );
	VectorInt INm( N );  
	Vector n_r( M );

	*m_outputScatteringFile << "N=" << N << " M=" << M << std::endl;
	*m_outputScatteringFile << "R_min=" << radiusBubbleMin << "�" << " R_max=" << radiusBubbleMax << "�" << std::endl;
	*m_outputScatteringFile << "h=" << h << "�" << std::endl;
	*m_outputScatteringFile << "eta = " << eta << std::endl;
	*m_outputScatteringFile << "����� ��������: " << Itr << " ������ �������� ����������: " << err << std::endl;
	*m_outputScatteringFile << "\n" << std::endl;

	*m_outputScatteringFile << "   ������ ������\n" << std::endl;
	*m_outputScatteringFile << "   ������� ��������\n" << std::endl;

	for ( size_t i = 0; i < L; i++)
	{
		*m_inputFile >> F_exp[i];       //  ������� ������������
		*m_outputScatteringFile << F_exp[i] << std::endl;
	}

	*m_outputScatteringFile << "   ���������������� �������� �����\n" << std::endl;
	for (size_t i = 0; i < Itr; ++i)
	{
		*m_inputFile >> Aph[i];
		*m_outputScatteringFile << Aph[i] << std::endl;
	}
	radiusNorm = radiusBubbleMin / radiusBubbleMax;

	deltaRadius = (1.0 - radiusNorm) / (N - 1);
	//   dr=(R_max-R_min)/(N-1);

	*m_outputScatteringFile << "\n" << std::endl;

	//   dd = 3.26*sqrt(1.0+0.098*h);   // for gas
	dd = 3.15*sqrt( 1.0 + 0.1*h );         // for methane
	double Lum; // �� ������� �������� ���� ����������

	for ( size_t i = 0; i < N; ++i)
	{
		radiusBubble = radiusNorm + deltaRadius*i;
		Lum = pow(radiusBubble, eta / 2.0)*exp(-radiusBubble / 2.0);

		Lm_r[0] = 1.0;
		Lm_r[1] = 1.0 + eta - radiusBubble;

		Umn[0][i] = Lum*(1.0 / sqrt(tgamma(eta + 1)));
		Umn[1][i] = Lum*((1.0 + eta - radiusBubble) / sqrt(tgamma(eta + 2)));

		for ( size_t j = 1; j < M; ++j )
		{
			Lm_r[j + 1] = ((2 * j + eta + 1.0 - radiusBubble)*Lm_r[j] - (eta + j)*Lm_r[j - 1]) / (j + 1);
			Umn[j + 1][i] = Lum*Lm_r[j + 1] * sqrt(tgamma(j + 1) / tgamma(j + eta + 1));

		}
	}

	// ���������� ����
	// ���������� �������������� � ��������
	double a, f, b, ka; // �� ������� �������� ���� ���������� (���� )
	double kapa = 2.0 * M_PI / averageSpeedSoundInSeaWater; // �� ������� �������� ���� ����������
	double kappa; // �� ������� �������� ���� ����������

	for ( size_t k = 0; k < L; ++k )
	{
		f = F_exp[k]; // �� ������� �������� ���� ����������
		kappa = kapa*f;

		for ( size_t i = 0; i < N; ++i )
		{
			radiusBubble = radiusNorm + deltaRadius * i;
			a = radiusBubble*radiusBubbleMax;
			ka = kappa * a;
			resonanceFreq = resonanceFrequency(a, dd);

			b = beta(ka);

			// The Medwin formula - the backscattering cross-section of a gas-filled

			Kmn_fr[k][i] = a * a / ((resonanceFreq * resonanceFreq / (f * f) - 1.0)*(resonanceFreq * resonanceFreq / (f * f) - 1.0 ) + b * b );
		}
	}

	   // ������ ������
	double ser = 0.0;    // ���������� ���� ������������

	for ( size_t k = 0; k < L; ++k )
	{

		for (  size_t j = 0; j < M; ++j )
		{
			Imn[j][0] = 0.0;

			for ( size_t i = 0; i<N; ++i )
			{
				ser = 0.5*deltaRadius*(Kmn_fr[k][i] * Umn[j][i] + Kmn_fr[k][i + 1] * Umn[j][i + 1]);
				Imn[j][i + 1] = Imn[j][i] + ser;
			}
			if (ser >= err)
			{
				printf("\n �������筠� �筮��� ���᫥���. ������� ���祭�� N �� ���冷�.\n");
				getchar();

				return;
			}
			Im_N[k][j] = Imn[j][N];

		}
	}

	double n_0 = 1200.0; // �� ������� �������� ���� ����������
	eta = 2.0; // �� ������� �������� ���� ����������
	a = 15.0; // �� ������� �������� ���� ����������
	

	Vector averageRadiusColl( N );
	for ( size_t i = 0; i<N; ++i )
	{
		averageRadiusColl[ i ] = radiusNorm + deltaRadius*i;

		Nm[i] = n_0*pow( averageRadiusColl[ i ], eta / 2.0)*exp(-averageRadiusColl[i] * a / 2.0);

		*m_outputScatteringFile << averageRadiusColl[i] << "  " << Nm[i] << std::endl;
	}
	// ����� �������� � ���������� �������� ��� ����
	/*
	������ ������� 2 ����� ����� ������ �������� ������, �������� �����������, ���������� ������� �������������
	� �� ��� �������� Nm[i]
	*/

	for ( size_t i = 0; i < M; ++i )
		*m_outputScatteringFile << "  " << Nm[i] << std::endl;

	for ( size_t k = 0; k < L; ++k )
	{
		mv_f[k] = 0.0;

		for ( size_t i = 0; i < M; ++i )
		{
			mv_f[k] += Nm[i] * Im_N[k][i];
		}

		*m_outputScatteringFile << "\n����������� ��������� ��������� �� ������� " << F_exp[k] << " = " << mv_f[k] << "\n" << std::endl;
	}

	*m_outputScatteringFile << "\n" << std::endl;

	Vector mv_fal( L ); // �� ������� �������� ���� ����������
	

	*m_outputScatteringFile << "\n   �������� ������\n" << std::endl;


	// ��������� ������� ������� � ��������� ������
	//   ��� ���� (����� ������������� �����������)

	for ( size_t j = 0; j < M; ++j)
	{
		B[j] = 0.0;

		for ( size_t i = 0; i < M; ++i)
		{
			A[j][i] = 0.0;

			for ( size_t k = 0; k < L; ++k )
			{
				A[j][i] += Im_N[k][i] * Im_N[k][j] / L;
			}
		}

		for ( size_t k = 0; k < L; ++k )
		{
			B[j] += mv_f[k] * Im_N[k][j];
		}
	}
	double D_min_alph = 1.0; // �� ������� �������� ���� ����������
	double ss; // �� �������  �������� ���� ����������

	for ( size_t iter = 0; iter < Itr; ++iter ) // Iteration
	{
		alpha = Aph[iter];

		*m_outputScatteringFile << " Alpha : = " << alpha << std::endl;

		for ( size_t j = 0; j < M; ++j )
			for ( size_t i = 0; i < M; ++i )
			{
				if (i == j)
					A[j][i] += alpha;

			}

		*m_outputScatteringFile << "\n �������� ������� [A]: " << std::endl;

		for (size_t i = 0; i < M ; ++i, std::cout << std::endl )
		{
			for (size_t j = 0; j < M; ++j)
			{
				*m_outputScatteringFile << A[i][j] << " ";
				*m_outputScatteringFile << std::endl;
			}
			*m_outputScatteringFile << "\n" << B[i] << "\n";
			*m_outputScatteringFile << std::endl;
		}

		//  ������� ����

		Gauss resolver(A, B);

		auto & X = resolver.resolveSystem();

		// ������ �����������
		*m_outputScatteringFile << "       ������� ����\n" << std::endl;

		if ( X.empty() )
			*m_outputScatteringFile << "������� �����������";
		else 
		{
			int free_vars_count = X[0].size() - 1;
			for (size_t i = 0; i < M; ++i ) 
			{
				*m_outputScatteringFile << "X[" << i + 1 << "]" << " = " << X[i].back();

				for ( size_t j = 0; j < X.rowSize(); ++j ) {
					if (fabs(X[i].back() > err) )
					{
						if ( X[i].back() > 0 )
							*m_outputScatteringFile << " + " << X[i][j] << "A[" << j + 1 << "]";
						else
							*m_outputScatteringFile << " - " << -X[i][j] << "A[" << j + 1 << "]";
					}
				}
				*m_outputScatteringFile << std::endl;
			}
		}

		for ( size_t k = 0; k < L; ++k )
		{
			mv_fal[k] = 0.0;

			for ( size_t i = 0; i < M; ++i)
			{
				Nm_s[i] = X[i].back();
				mv_fal[k] += Nm_s[i] * Im_N[k][i];
			}
			*m_outputScatteringFile << "\n����������� ��������� ��������� �������� ������ - " << mv_fal[k] << std::endl;
		}

		double	SZ = 0.0, SCH = 0.0; // �� ������� �������� ���� ����������

		for ( size_t i = 0; i < L; ++i )
		{
			SZ += mv_f[i] * mv_f[i];
			SCH += (mv_f[i] - mv_fal[i])*(mv_f[i] - mv_fal[i]);
		}

		delta_al[iter] = SCH / SZ;

		if (delta_al[iter] <= D_min_alph)
		{
			*m_outputGraphicsFile << "\n Alpha = " << alpha << " Delta-alpha = " << delta_al[iter] << "\n" << std::endl;
			for ( size_t k = 0; k < M; ++k )
			{
				ss = 0.0;
				for ( size_t i = 0; i < M; ++i )
				{
					ss += Nm_s[i] * Umn[i][k];
				}
				n_r[k] = ss;
				*m_outputGraphicsFile << "  n_r = " << ss << std::endl;
			}
		}

	}

	for ( size_t i = 0; i<M; ++i)
		*m_outputGraphicsFile << "  n_r[" << i << "] " << n_r[i] << std::endl;

} // PhisicalModel::calculatePhisicalModel

/*---------------------------------------------------------------------------*/