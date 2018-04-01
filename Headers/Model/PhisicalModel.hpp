/*
    2018

    Author : Kirill Zharenkov, Aldec-KTC Team.

    Date creation: 20.01.2018

    Purpose: class for representation phisical model.

    All right reserved (c).

*/

/*---------------------------------------------------------------------------*/

#ifndef PHISICAL_MODEL_HPP_
#define PHISICAL_MODEL_HPP_

/*---------------------------------------------------------------------------*/

#include "Headers/ph/ph.hpp"
#include "Headers/Model/Gauss.hpp"
#include "Headers/Utils/Log.hpp"

/*---------------------------------------------------------------------------*/

class PhisicalModel
{

/*---------------------------------------------------------------------------*/

    using LogImplPtr = std::unique_ptr< Log >;

    using Table = std::vector<
            std::pair<
                    double
                  , double
             >
    >;

    using Vector = Gauss::Vector;


/*---------------------------------------------------------------------------*/

    const double averageSpeedSoundInSeaWater = 1.522E5;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    using ChartData = std::tuple<
            Table
          , Gauss::Vector
          , Gauss::Vector
          , Gauss::Vector
    >;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    PhisicalModel() = default;

    PhisicalModel(
         const std::string& _inputFilePath
      ,  const std::string& _inputStatFilePath
    );

    ~PhisicalModel();

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

   ChartData calculatePhisicalModel();

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    double  beta( double _scalar ) const noexcept;

    double resonanceFrequency( double _radius, double _d ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  auto makeLogImpl(
        const std::string& _inputFile
      , const std::string& _inputStatFile
  ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    LogImplPtr m_log;

/*---------------------------------------------------------------------------*/

}; // class PhisicalModel

 /*---------------------------------------------------------------------------*/

inline double
PhisicalModel::beta( double _scalar ) const noexcept
{
        return _scalar * _scalar;

} // PhisicalModel::beta

/*---------------------------------------------------------------------------*/

inline double
PhisicalModel::resonanceFrequency( double _radius, double _d ) const noexcept
{
        return _radius / _d;

} // PhisicalModel::resonanceFrequency

/*---------------------------------------------------------------------------*/

inline auto
PhisicalModel::makeLogImpl(
    const std::string& _inputFilePath
  , const std::string& _inputStatFilePath
) const noexcept
{
    return std::make_unique< Log >(_inputFilePath, _inputStatFilePath );

} // PhisicalModel::makeLogImpl

/*---------------------------------------------------------------------------*/

#endif // !PHISICAL_MODEL_HPP
