/*
    2018

    Author : Kirill Zharenkov, Aldec-KTC Team.

    Date: 01.11.2017

    Purpose: Class for representation  Gaussian elimination. An algorithm for solving systems of linear equations.

    All right reserved (c).

*/

/*---------------------------------------------------------------------------*/

#ifndef GAUSS_HPP_
#define GAUSS_HPP_

#include "Headers/ph/ph.hpp"
#include "Headers/Model/Matrix.hpp"

/*---------------------------------------------------------------------------*/

class Gauss
        : public boost::noncopyable
{

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    using Vector = Matrix::Row;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    using VectorPtr = std::unique_ptr< Vector >;
    using MatrixPtr = std::unique_ptr< Matrix >;

/*---------------------------------------------------------------------------*/

    bool isValid() const noexcept;

    template < typename _DataType >
    auto makeData( const _DataType& _rawData ) const noexcept;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    Gauss( const Matrix& _matrix, const Vector& _vector );

    ~Gauss() = default;

    Vector getRoots();

    Vector normalGaussianDestribution(
                unsigned _size, double _am
            ,   double _psi
            ,   double _mju
     ) const;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    const Matrix& resolveSystem();

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    MatrixPtr m_matrix;

    VectorPtr m_vector;

/*---------------------------------------------------------------------------*/

}; // class Gauss

/*---------------------------------------------------------------------------*/

template< typename _DataType >
auto
Gauss::makeData( const _DataType & _rawData ) const noexcept
{
    return std::make_unique< _DataType >( _rawData );

} // Gauss::makeData< _DataType >

/*---------------------------------------------------------------------------*/

#endif // !GAUSS_HPP_
