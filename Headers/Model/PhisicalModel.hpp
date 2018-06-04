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
#include "Headers/Utils/Defines.hpp"

/*---------------------------------------------------------------------------*/

class PhisicalModel
{

/*---------------------------------------------------------------------------*/

    using LogImplPtr = std::unique_ptr< Log >;

    using Vector = Gauss::Vector;

/*---------------------------------------------------------------------------*/

    const double averageSpeedSoundInSeaWater { 1.522E5 };

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    using SetupCollImpl = std::map< const size_t, QString >;

    using SetupCollImplPtr = std::unique_ptr< SetupCollImpl >;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    PhisicalModel() = default;

    PhisicalModel(
         const SetupCollImplPtr& _setup
      ,  const std::string& _inputStatFilePath
    );

    ~PhisicalModel();

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

   Defines::ChartData calculatePhisicalModel();

   SetupCollImpl& getSetupData() const;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    double  beta( double _scalar ) const noexcept;

    double resonanceFrequency( double _radius, double _d ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  auto makeLogImpl( const std::string& _inputStatFile ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

   LogImplPtr m_log;

   SetupCollImpl& m_setup;

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
PhisicalModel::makeLogImpl( const std::string& _inputStatFilePath ) const noexcept
{
    return std::make_unique< Log >( _inputStatFilePath );

} // PhisicalModel::makeLogImpl

/*---------------------------------------------------------------------------*/

#endif // !PHISICAL_MODEL_HPP
