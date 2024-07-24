
#include "Views/CharBuilderDirector.hpp"

/*---------------------------------------------------------------------------*/

ChartBuilderDirector::ChartBuilderDirector( PhysicalModel& _model )
    :   m_builder( makeBuilderImplPtr( _model ) )
{
} // ChartBuilderDirector::ChartBuilderDirector

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilderDirector::getHistogramChart( const Defines::Table& _table ) const
{

    return m_builder->getHistohramChart( _table );

} // ChartBuilderDirector::getHistogramChart

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilderDirector::getLineChart() const
{
    return m_builder->getLineChart();

} // ChartBuilderDirector::getLineChart

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilderDirector::getEchoSignalsChart() const
{
    return m_builder->geEchoSignalsChart();

}

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilderDirector::getDiscrepencyChart() const
{
    return m_builder->getDiscrepencyChart();

}


/*---------------------------------------------------------------------------*/

void
ChartBuilderDirector::construct()
{
    m_builder->build();

} // ChartBuilderDirector::construct

/*---------------------------------------------------------------------------*/
