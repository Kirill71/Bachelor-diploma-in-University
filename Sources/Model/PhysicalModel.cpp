#include "Model/PhysicalModel.hpp"

/*---------------------------------------------------------------------------*/

PhysicalModel::PhysicalModel(
        const SetupCollImplPtr& _setup
    ,   const std::string & _inputStatFilePath
)
    :   m_log (  makeLogImpl( _inputStatFilePath ) )
    ,   m_setup( *_setup )
{
} // PhysicalModel::PhysicalModel

/*---------------------------------------------------------------------------*/

PhysicalModel::~PhysicalModel()
{
    m_log.reset();

} // PhysicalModel::PhysicalModel

/*---------------------------------------------------------------------------*/

Defines::ChartData
PhysicalModel::calculatePhisicalModel()
{
    using VectorInt = std::vector < int >;

    auto log = [ this ]( Log::FileType _type = Log::FileType::Scattering )-> auto&
    {
        return m_log->log( _type );
    };

    size_t N, M, L, Itr;            // размерность массивов

    double    resonanceFreq,   // Резонансная частота
                radiusNorm,			// нормированный радиус пузырька
                radiusBubbleMin,    // R_min < radiusBubble
                radiusBubbleMax,    // radiusBubble > R_max
                depth,				 // указатель глубины
                demfRatio,                // teta > 0  // не понятно, название этой переменной
                deltaRadius,				 // дельта прирашения по радиусу - не понятно, название этой переменной
                dd;				 // не понятно, название этой переменной

    double alpha, err;

    N = m_setup[ IntegralSeparating ].toUInt();
    M = m_setup[ SystemSize ].toUInt();
    L = m_setup[ SoundingFreqNumber ].toUInt();
    radiusBubbleMin = m_setup[ RadiusMin ].toDouble();
    radiusBubbleMax = m_setup[ RadiusMax ].toDouble();
    depth = m_setup[ Depth ].toDouble();
    demfRatio = m_setup[ DemfRatio].toDouble();
    Itr = m_setup[ AlfaNumber ].toUInt();
    err = m_setup[Epsilon].toDouble();

    // in the new method
    Defines::Table table;
    while( !log( Log::FileType::Stat ).eof() )
    {
        double first, second;
        log( Log::FileType::Stat ) >> first >> second;
        table.push_back( std::make_pair( first, second ) );
    };

    table.erase( --table.end() );

    Vector frecExp( L );  // массив частот эксперимента
    Vector alphaVector( Itr );  // массив для поиска альфы, далее будет замена на оптимизирующий алгоритм
    Vector delta_al( Itr );  // массив критериев отбора

    Matrix systemMatrix( M , M );  // матрица для СЛАУ
    Vector freeMembers( M );  // вектор правой части
    Vector mv_f( L );  // то, что мы должны приблизить к решению

    Matrix basisFuncMatrix( M + 1, N + 1 );  // матрица базисных функций
    Matrix integralBasisFancMatrix( M + 1, N + 1 );  // матрица интеграла от б.ф
    Matrix integralFuncCernalMatrix( L, N + 1 );  // матрица ядра подинтегральной функции
    Vector polinomsMatrix( M + 1 );  // полиномы
    Matrix integrationMatrix( L, M + 1 );  // матрица интегрирования

          /*  массивы для посчетов количества пузырьков */
    Vector Nm( N );
    Vector Nm_s( M );
    VectorInt INm( N );
    Vector n_r( N );
    Matrix XA( Itr , M );

    log() << "N=" << N << " M=" << M << std::endl;
    log() << "R_min=" << radiusBubbleMin << "м" << " R_max=" << radiusBubbleMax << "м" << std::endl;
    log() << "h=" << depth << "м" << std::endl;
    log() << "eta = " << demfRatio << std::endl;
    log() << "Число инераций: " << Itr << " Ошибка точности вычислений: " << err << std::endl;
    log() << "\n" << std::endl;

    log() << "   Прямая задача\n" << std::endl;
    log() << "   Частоты контроля\n" << std::endl;

    auto str =  m_setup[ SoundingFrequensies ];
    for ( size_t i = 0; i < L; ++i)
    {
        frecExp[i]
               = m_setup[ SoundingFrequensies ]
                    .split( "," )[ static_cast<int>( i ) ].toDouble();

        log() << frecExp[i] << std::endl;
    }

    log() << "   Регуляризирующие значения альфа\n" << std::endl;
    for (size_t i = 0; i < Itr; ++i)
    {
        alphaVector[ i ]
               = m_setup[ RegularAlfaValue ]
                    .split(",")[ static_cast<int>( i ) ].toDouble();

        log() << alphaVector[i] << std::endl;
    }
    radiusNorm = radiusBubbleMin / radiusBubbleMax;

    deltaRadius = (1.0 - radiusNorm) / (N - 1);

     // in the new method. mb too.
    Vector radiusCollection( N );
    for (size_t i = 0; i < N; ++i )
        radiusCollection[ i ] = radiusNorm + deltaRadius * i;

    log() << std::endl << std::endl;

    dd = 10.6276*(1.0+0.00985*depth);

    double a,f,b;

    a = 1.2;
    b = 7.5;

    for ( size_t i = 0; i < N; ++i )
    {
        for (size_t j=0; j<M; ++j )
            basisFuncMatrix[ j ][ i ] =
                exp( -b *( j + 1 ) * radiusCollection[ i ] ) * pow( radiusCollection[ i ] , a );
    }

  // Вычисление ядра


    b = 0.32;
    double squareRadius( 0.0 );
    for ( size_t k = 0; k < L; ++k )
    {
        f = frecExp[k]; // не понятно название этой переменной

        for ( size_t i = 0; i < N; ++i )
        {
            squareRadius =  radiusCollection[ i ] * radiusCollection[ i ];
            resonanceFreq = dd/(radiusBubbleMax*radiusBubbleMax* squareRadius );

            // The Medwin formula - the backscattering cross-section of a gas-filled

            integralFuncCernalMatrix[k][i] = squareRadius / ((resonanceFreq / (f * f) - 1.0)*(resonanceFreq / (f * f) - 1.0 ) + b * b );
        }
    }

     // Прямая задача
    double ser = 0.0;    // Остаточный член суммирования

    for ( size_t k = 0; k < L; ++k )
    {
        for (  size_t j = 0; j < M; ++j )
        {
            integralBasisFancMatrix[j][0] = 0.0;
            for ( size_t i = 0; i<N; ++i )
            {
                ser = 0.5*deltaRadius*(integralFuncCernalMatrix[k][i] * basisFuncMatrix[j][i] + integralFuncCernalMatrix[k][i + 1] * basisFuncMatrix[j][i + 1]);
                integralBasisFancMatrix[j][i + 1] = integralBasisFancMatrix[j][i] + ser;
            }
            integrationMatrix[k][j] = integralBasisFancMatrix[j][N];
        }
    }

    double n_0 = 120.0; // не понятно название этой переменной
    demfRatio = 2.7;

    a = 15.0;

    for ( size_t i = 0; i<N; ++i )
    {
        Nm[i] = n_0*pow(radiusCollection[i], demfRatio / 2.0)*exp(-radiusCollection[i]*a / 2.0);
        log() << radiusCollection[ i ] << "  " << Nm[i] << std::endl;
    }
  // Конец загрузки и подготовки массивов для СЛАУ
  /*
  Вместо примера 2 нужно будет читать исходные данные, рисовать гистограмму, определять функцию распределения
  и по ней находить Nm[i]
  */

    for ( size_t k = 0; k < L; ++k )
    {
        mv_f[k] = 0.0;
        for (size_t i=0; i<N; ++i )
            mv_f[ k ] += 0.5 * deltaRadius * ( integralFuncCernalMatrix[ k ][ i ] * Nm[ i ] + integralFuncCernalMatrix[ k ][ i+1 ] * Nm[ i+1 ] );

        log() << "\nКоэффициент объемного рассеяния на частоте " << frecExp[k] << " = " << mv_f[k] << "\n" << std::endl;
    }

    log() << "\n" << std::endl;

    Vector mv_fal( L ); // не понятно название этой переменной

    log() << "\n   Обратная задача\n" << std::endl;


  // Формируем матрицу системы и свободный вектор
  //   для СЛАУ (метод регуляризации Лаврентьева)

    for ( size_t j = 0; j < M; ++j )
    {
        freeMembers[j] = 0.0;
        for ( size_t i = 0; i < M; ++i )
        {
            systemMatrix[j][i] = 0.0;
            for ( size_t k = 0; k < L; ++k )
                systemMatrix[j][i] += integrationMatrix[k][i] * integrationMatrix[k][j] / L;
        }

        for ( size_t k = 0; k < L; ++k )
            freeMembers[j] += mv_f[k] * integrationMatrix[k][j];
    }

    double alpha_ = 0.0;
    for ( size_t iter = 0; iter < Itr; ++iter ) // Iteration
    {
        alpha = alphaVector[iter];
        log() << " Alpha : = " << alpha << std::endl;

         for ( size_t i = 0; i < M; ++i )
            systemMatrix[i][i] += alpha-alpha_;
         alpha_ = alpha;

         log() << "\n Исходная матрица [A]: " << std::endl;

         for (size_t i = 0; i < M ; ++i, std::cout << std::endl )
         {
            for (size_t j = 0; j < M; ++j)
            {
                log() << systemMatrix[i][j] << " ";
                log() << std::endl;
            }

            log() << "\n" << freeMembers[i] << "\n";
            log() << std::endl;
         }

      //  Решение СЛАУ

        Gauss resolver(systemMatrix, freeMembers);
        auto X = resolver.getRoots();

      // Анализ результатов
        log() << "       Решение СЛАУ\n" << std::endl;

        if ( X.empty() )
          log() << "Система несовместна";
        else
        {
            for (size_t i = 0; i < M; ++i )
            {
                log() << "X[" << i + 1 << "]" << " = " << X[ i ];
                if  ( fabs(X[ i ] ) > err )
                {
                    if ( X[ i ] > 0 )
                        log() << " + " << X[ i ] << "A[" << i + 1 << "]";
                     else
                        log() << " - " << -X[ i ] << "A[" << i + 1 << "]";
                 }
             }
             log() << std::endl;
       }

       for ( size_t k = 0; k < L; ++k )
       {
            mv_fal[ k ] = 0.0;

            for ( size_t i = 0; i < M; ++i)
            {
                Nm_s[i] = X[i];
                mv_fal[k] += Nm_s[i] * integrationMatrix[k][i];
            }
            log() << "\nКоэффициент объемного рассеяния обратной задачи - " << mv_fal[k] << std::endl;
        }

        double	SZ = 0.0, SCH = 0.0; // не понятно название этой переменной

        for ( size_t i = 0; i < L; ++i )
        {
            SZ += mv_f[ i ] * mv_f[ i ];
            SCH += ( mv_f[ i ] - mv_fal[ i ] ) * ( mv_f[ i ] - mv_fal[ i ] );
        }

        delta_al[iter] = SCH / SZ;

        for ( size_t i = 0; i < M; ++i )
          XA[ iter ][ i ] = X[ i ];

    } // End Iterasion for alpha

    double D_min = *std::min_element( delta_al.cbegin(), delta_al.cend());
    double D_max = *std::max_element( delta_al.cbegin(), delta_al.cend());

    log() << " Min distance: " << D_min << " Max distance: " << D_max << std::endl;

    auto minDistance = static_cast< unsigned long >( std::distance( delta_al.cbegin(), std::min_element( delta_al.cbegin(), delta_al.cend() ) ) );

    log() << " Alpha = " << alphaVector[ minDistance ] << " Min distance: " << minDistance << std::endl;

    log( Log::FileType::Chart ) << "\n Alpha = " << alphaVector[ minDistance ] << " Delta-alpha = " << delta_al[ minDistance ] << "\n" << std::endl;
    for ( size_t k = 0; k < N; ++k )
    {
        for ( size_t i = 0; i < M; ++i )
            n_r[k] += XA[minDistance][i] * basisFuncMatrix[i][k];
    }

    for ( size_t i = 0; i<N; ++i)
        log( Log::FileType::Chart ) << radiusCollection[ i ] << " " << n_r[ i ] << std::endl;

    return { table, radiusCollection, Nm, n_r, mv_f, frecExp, delta_al, alphaVector };

} // PhysicalModel::calculatePhisicalModel

/*----------------------------------------------------------------------------*/

PhysicalModel::SetupCollImpl&
PhysicalModel::getSetupData() const
{
    return m_setup;

} // PhysicalModel::getSetupData()

/*----------------------------------------------------------------------------*/
