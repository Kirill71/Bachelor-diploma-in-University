
#include "Headers/Views/ChartBuilder.hpp"

ChartBuilder::ChartBuilder( PhisicalModel& _model  )
    :  m_model( _model )
{

}

void
ChartBuilder::build()
{
    auto data = m_model.calculatePhisicalModel();
    buildHistohramChart( data );
    buildLineChart(data );

}


void
ChartBuilder::buildHistohramChart()
{
   auto table = std::get< 0 >( chartData );

}
