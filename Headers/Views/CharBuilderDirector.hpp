#pragma once

/*---------------------------------------------------------------------------*/

#include "Views/Builder.hpp"
#include "Views/ChartBuilder.hpp"

/*---------------------------------------------------------------------------*/

class ChartBuilderDirector
{

/*---------------------------------------------------------------------------*/

    using BuilderImplPtr = std::unique_ptr< Builder >;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    explicit ChartBuilderDirector(PhysicalModel& _model );

    QChart* getHistogramChart( const Defines::Table& _table ) const;

    QChart* getLineChart() const;

    QChart* getEchoSignalsChart() const;

    QChart* getDiscrepencyChart() const;

    void construct();

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    auto makeBuilderImplPtr(PhysicalModel& _model ) const;

/*---------------------------------------------------------------------------*/

    BuilderImplPtr m_builder;

/*---------------------------------------------------------------------------*/

}; // class ChartBuilderDirector

/*---------------------------------------------------------------------------*/

inline auto
ChartBuilderDirector::makeBuilderImplPtr(PhysicalModel& _model ) const
{
    return std::make_unique< ChartBuilder >( _model );
}
