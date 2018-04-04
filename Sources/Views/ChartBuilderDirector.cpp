
#include "Headers/Views/CharBuilderDirector.hpp"

/*---------------------------------------------------------------------------*/

ChartBuilderDirector::ChartBuilderDirector( PhisicalModel& _model )
    :   m_builder( makeBuilderImplPtr( _model ) )
{
    construct();

} // ChartBuilderDirector::ChartBuilderDirector

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilderDirector::getHistogramChart() const
{
    return m_builder->getHistohramChart();

} // ChartBuilderDirector::getHistogramChart

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilderDirector::getLineChart() const
{
    return  m_builder->getLineChart();

} // ChartBuilderDirector::getLineChart

/*---------------------------------------------------------------------------*/

void
ChartBuilderDirector::construct()
{
    m_builder->build();

} // ChartBuilderDirector::construct

/*---------------------------------------------------------------------------*/
