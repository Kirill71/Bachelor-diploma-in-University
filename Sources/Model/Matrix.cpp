#include "Model/Matrix.hpp"

/*---------------------------------------------------------------------------*/

auto
Matrix::makeMatrixImpl( size_t _rowSize ) const noexcept
{
    return std::make_unique< Matrix::MatrixImpl >( _rowSize );

} // Matrix::makeMatrixImpl

/*---------------------------------------------------------------------------*/

Matrix::Matrix( size_t _rowSize, size_t _collSize )
    :	m_rowSize( _rowSize )
    ,	m_collSize( _collSize )
    ,	m_matrix( makeMatrixImpl( m_rowSize ) )
{

    std::for_each(
            begin()
        ,	end()
        ,	[ = ]( Matrix::Row& _row )
            {
                _row.resize( m_collSize );
            }
    );

} // Matrix::Matrix

/*---------------------------------------------------------------------------*/

Matrix::Matrix( const Matrix& _matrix )
    :	Matrix(
                _matrix.rowSize()
            ,	_matrix.collSize()
        )
{
    std::copy(
            _matrix.cbegin()
        ,	_matrix.cend()
        ,	begin()	);

} // Matrix::Matrix

/*---------------------------------------------------------------------------*/

Matrix::Matrix(
        size_t _rowSize
    ,	size_t _collSize
    ,	std::initializer_list< MatrixDataType > _list
) : Matrix( _rowSize, _collSize )
{
    if ( collSize() * rowSize() < _list.size() )
        throw std::logic_error(" bad size of elements"); // add custom exception

    auto it = _list.begin();

    std::for_each(
            m_matrix->begin()
        ,	m_matrix->end()
        ,	[ & ]( Row& _row )
            {
                std::for_each(
                        _row.begin()
                    ,	_row.end()
                    ,	[ & ]( MatrixDataType& _data )
                        {
                            _data = *it++;
                        }
                );
            }
    );

} // Matrix::Matrix

/*---------------------------------------------------------------------------*/

Matrix&
Matrix::operator= ( const Matrix& _matrix )
{
    if (this != &_matrix )
    {
        this->~Matrix();
        new ( this ) Matrix( _matrix );
    }

    return *this;

} // Matrix::operator=

/*---------------------------------------------------------------------------*/

void
Matrix::insertRow( const Matrix::Row& _row )
{
    if ( ( *m_matrix )[0].size() != _row.size() )
        return;

    m_matrix->push_back( _row );

    ++m_rowSize;

} // Matrix::insertRow

/*---------------------------------------------------------------------------*/

void
Matrix::insertColumn( const Matrix::Column& _column )
{
    if ( m_matrix->size() != _column.size() )
        return;

    for ( size_t i = 0; i < rowSize(); ++i)
        (*m_matrix)[i].push_back( _column[i] ); // todo improve

    ++m_collSize;

} // Matrix::insertColumn


/*---------------------------------------------------------------------------*/

const Matrix::Row&
Matrix::operator[] ( size_t _index ) const
{
    return (*m_matrix )[ _index ];

} // Matrix::operator[]

/*---------------------------------------------------------------------------*/

Matrix::Row&
Matrix::operator[] ( size_t _index )
{
    return const_cast< Matrix::Row& >(
        static_cast< const Matrix& >( *this )
        [ _index ]
    );

} // Matrix::operator[]

/*---------------------------------------------------------------------------*/

void
Matrix::clear()
{
    m_matrix->clear();

} // Matrix::clear

/*---------------------------------------------------------------------------*/

Matrix::Row
operator+ ( const Matrix::Row& _lhs, const Matrix::Row & _rhs )
{
    Matrix::Row result;

    std::transform(
            _lhs.begin()
        ,	_lhs.end()
        ,	_rhs.begin()
        ,	std::back_inserter( result )
        ,	std::plus< Matrix::MatrixDataType >()
    );

    return result;

} // operator+

/*---------------------------------------------------------------------------*/

Matrix::Row
operator* ( const Matrix::Row & _row, const Matrix::MatrixDataType _scalar )
{
    using namespace std::placeholders;

    Matrix::Row  result;

    std::transform(
            _row.begin()
        ,	_row.end()
        ,	result.begin()
        ,	std::bind( std::multiplies< Matrix::MatrixDataType >(), _1, _scalar )
    );

    return result;

} // operator*

/*---------------------------------------------------------------------------*/

std::ostream &
operator<< ( std::ostream & _ostream, const Matrix & _matrix )
{
    std::for_each(
            _matrix.cbegin()
        ,	_matrix.cend()
        ,	[ & ]( const Matrix::Row& _row )
            {
                std::for_each(
                        _row.cbegin()
                    ,	_row.cend()
                    ,	[ & ]( Matrix::MatrixDataType _value )
                        {
                            _ostream << _value << " ";
                        }
                );
                _ostream << std::endl;

            }
    );

    return _ostream;

} // operator<<


/*---------------------------------------------------------------------------*/

const std::istream&
operator>> ( std::istream & _istream, Matrix & _matrix )
{

    std::for_each(
            _matrix.cbegin()
        ,	_matrix.cend()
        ,	[ & ]( const Matrix::Row& _row )
            {
                std::for_each(
                        _row.cbegin()
                    ,	_row.cend()
                    ,	[ & ]( Matrix::MatrixDataType _value )
                        {
                            _istream >> _value ;
                        }
                );
            }
    );

    return _istream;

} // operator>>

/*---------------------------------------------------------------------------*/
