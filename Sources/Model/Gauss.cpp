#include "Model/Gauss.hpp"

/*---------------------------------------------------------------------------*/

Gauss::Gauss( const Matrix & _matrix, const Vector & _vector )
	: m_matrix( makeData< Matrix >( _matrix ) )
	, m_vector( makeData< Vector >( _vector ) )
{

	m_matrix->insertColumn( *m_vector );

} // Gauss::Gauss

/*---------------------------------------------------------------------------*/

bool
Gauss::isValid() const noexcept
{
	if (
		m_matrix->empty()
		|| (*m_matrix)[0].empty()
		|| m_matrix->rowSize() != m_vector->size()
		)	return false;

	size_t firstRowSize{ (*m_matrix)[0].size() };
	std::for_each(
		m_matrix->cbegin()
		, m_matrix->cend()
		, [ & ]( const Matrix::Row& _row )
	{
		return _row.size() != firstRowSize;
	}
	);

	return true;

} //  Gauss::isValid

/*---------------------------------------------------------------------------*/

const Matrix&
Gauss::resolveSystem()
{
	if ( isValid() )
	{
		auto & matrix{ *m_matrix };

		const size_t MatrixSize = matrix.rowSize();

		size_t sameVariable = 0;

		/*-------------------------------------------------
					 lambda definitions
		-------------------------------------------------*/

		auto equal = [&](double _lhs, double _rhs) -> bool
		{
			const double EPS = 1e-12;
			return fabs(_rhs - _lhs) <= EPS;
		};

		/*-------------------------------------------------*/

		auto swapRowWithZero = [ & ]( size_t _k )
		{
			double maxAbs;
			size_t rowMaxPos = _k;
			size_t j = 0;

			while (j <= MatrixSize - 1 )
			{
				maxAbs = 0.0;

				for ( size_t i = _k; i < MatrixSize; ++i)
				{
					maxAbs = std::max( fabs( matrix[i][j]), maxAbs );
					rowMaxPos = i;
				}
				if (!equal(maxAbs, 0))
					break;
				++j;
			}
			if (j > MatrixSize - 1)
				return;

			++sameVariable;

			if (rowMaxPos != _k && matrix[_k][_k] == 0.0 )
			{
				for ( size_t i = j; i < MatrixSize; ++i )
					std::swap( matrix[_k][i], matrix[rowMaxPos][i]);
			}
		};

		/*--------------------------------------------------------*/

		auto divideEquation = [ & ]( size_t _k )
		{
			auto divisor = matrix[ _k ][ _k ];
			for ( size_t i = 0; i < MatrixSize + 1; ++i)
				matrix[_k][i] /= divisor;
		};

		/*------------------------------------------------------*/

		auto dedactEquation = [&]( size_t _k )
		{
			for (size_t i = 0; i < MatrixSize; ++i)
			{
				if (i != _k)
				{
					auto multiplier = matrix[i][_k];
					for (size_t j = 0; j < MatrixSize + 1; ++j)
						matrix[ i ][ j ] -= multiplier * matrix[ _k ][ j ];
				}
			}
		};

		/*-----------------------------------------------------*/

		for ( size_t k = 0; k < MatrixSize; ++k )
		{
			swapRowWithZero( k );

			divideEquation( k );

			dedactEquation( k );
		}


		if (sameVariable < MatrixSize) // check system compatebility
			matrix.clear();
	}

	return *m_matrix;

} // Gauss::resolveSystem

/*---------------------------------------------------------------------------*/

Gauss::Vector
Gauss::getRoots()
{
	Vector roots;
	resolveSystem();

	std::for_each(
			m_matrix->cbegin()
		,	m_matrix->cend()
		,	[ & ]( const Matrix::Row& _row )
			{
				roots.push_back( _row.back() );
			}
	);

	return roots;

} // Gauss::getRoots

/*---------------------------------------------------------------------------*/

Gauss::Vector
Gauss::normalGaussianDestribution(
	unsigned _size
,	double _am
,	double _psi
,	double _mju
) const
{
	Vector gas( _size );

	double bubble = _am * _psi * sqrt( 2.0 * M_PI );
	double x = 0.0;

	double dx = 4.0 / ( _size - 1 );

	for ( unsigned i = 0; i< _size; ++i)
	{
		x += dx * i;
		gas[ i ] = floor( bubble * exp( -( x - _mju )*( x - _mju ) / ( 2.0 *_psi * _psi) ) );
	}

	return gas;

} //

/*---------------------------------------------------------------------------*/
