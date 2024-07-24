/*
    2018

    Author : Kirill Zharenkov, Aldec-KTC Team.

    Date creation: 20.01.2018

    Purpose: class for representation physical model.

    All right reserved (c).

*/

/*---------------------------------------------------------------------------*/

#pragma once

/*---------------------------------------------------------------------------*/

#include "ph/ph.hpp"
#include "Model/Gauss.hpp"
#include "Utils/Log.hpp"
#include "Utils/Defines.hpp"

/*---------------------------------------------------------------------------*/

class PhysicalModel final
{

/*---------------------------------------------------------------------------*/

    using LogImplPtr = std::unique_ptr< Log >;

    using Vector = Gauss::Vector;

/*---------------------------------------------------------------------------*/

    static constexpr double averageSpeedSoundInSeaWater { 1.522E5 };

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    using SetupCollImpl = std::map< const size_t, QString >;

    using SetupCollImplPtr = std::unique_ptr< SetupCollImpl >;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    PhysicalModel(
         const SetupCollImplPtr& _setup
      ,  const std::string& _inputStatFilePath
    );

    ~PhysicalModel();

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

}; // class PhysicalModel

 /*---------------------------------------------------------------------------*/

inline double
PhysicalModel::beta(double _scalar ) const noexcept
{
        return _scalar * _scalar;

} // PhysicalModel::beta

/*---------------------------------------------------------------------------*/

inline double
PhysicalModel::resonanceFrequency(double _radius, double _d ) const noexcept
{
        return _radius / _d;

} // PhysicalModel::resonanceFrequency

/*---------------------------------------------------------------------------*/

inline auto
PhysicalModel::makeLogImpl(const std::string& _inputStatFilePath ) const noexcept
{
    return std::make_unique< Log >( _inputStatFilePath );

} // PhysicalModel::makeLogImpl

/*---------------------------------------------------------------------------*/
