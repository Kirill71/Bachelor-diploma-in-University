
#include "Headers/Views/ChartBuilder.hpp"

/*---------------------------------------------------------------------------*/

ChartBuilder::ChartBuilder( PhisicalModel& _model  )
    :   m_histohramChart( new QChart() )
    ,   m_lineChart( new QChart() )
    ,   m_model( _model )
{
} // ChartBuilder::ChartBuilder

/*---------------------------------------------------------------------------*/

void
ChartBuilder::build()
{
    auto data = m_model.calculatePhisicalModel();
    buildHistohramChart( data );
    buildLineChart(data );

} // ChartBuilder::build

/*---------------------------------------------------------------------------*/

void
ChartBuilder::buildHistohramChart( const PhisicalModel::ChartData& _data )
{
   auto table = std::get< 0 >( _data );
   std::vector< QBarSet* > sets;

   auto histSeries = new QBarSeries;
   for ( auto const & row : table )
   {
       sets.push_back( new QBarSet( std::to_string( row.first ).substr(0,3).c_str() ) );
       *sets.back() << row.second;
       histSeries->append( sets.back() );
   }

    m_histohramChart->addSeries( histSeries );
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(
            0
       ,    std::max_element(
                    table.cbegin()
                ,    table.cend()
                ,    []( auto _lhs, auto _rhs )
                     {
                        return _lhs.second < _rhs.second;
                     }
            )->second
    );

    axisY->setTickCount( 7 );
    m_histohramChart->setAxisY( axisY, histSeries );
    m_histohramChart->legend()->setVisible( true );
    m_histohramChart->legend()->setAlignment( Qt::AlignBottom );

    m_histohramChart->setAnimationOptions( QChart::SeriesAnimations );

} // ChartBuilder::buildHistohramChart

/*---------------------------------------------------------------------------*/

void
ChartBuilder::buildLineChart( const PhisicalModel::ChartData& _data )
{
    auto radiusCollection = std::get< 1 >( _data );
    auto nmCollection = std::get< 2 >( _data );
    auto nrCollection = std::get< 3 > ( _data );

    QLineSeries *series = new QLineSeries();
    for ( size_t i = 0; i < nmCollection.size(); ++i )
    {
        *series << QPointF( radiusCollection[ i ], nmCollection[ i ] );
        series->setName("input chart");
    }

    m_lineChart->addSeries(series);

    series = new QLineSeries();
    for ( size_t i = 0; i< nrCollection.size(); ++i )
    {
        *series << QPointF(radiusCollection[ i ], nrCollection[ i ] );
        series->setName("output chart");
    }

    m_lineChart->setAnimationOptions( QChart::SeriesAnimations );

    m_lineChart->addSeries(series);
    m_lineChart->createDefaultAxes();
    m_lineChart->axisX()->setMin(0.0);
    m_lineChart->axisX()->setMax(*std::max_element( radiusCollection.begin(), radiusCollection.end() ) );
    m_lineChart->axisY()->setMin(0.0);
    m_lineChart->axisY()->setMax(
        std::max(
                *std::max_element(
                        nmCollection.begin()
                    ,   nmCollection.end()
                )
        ,       *std::max_element(
                        nrCollection.begin()
                    ,   nrCollection.end()
                )
        ) + 0.015
    );

    m_lineChart->legend()->setVisible( true );
    m_lineChart->legend()->setAlignment( Qt::AlignBottom );
    m_lineChart->legend()->attachToChart();

} // ChartBuilder::buildLineChart

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilder::getHistohramChart() const
{
    return m_histohramChart;

} // ChartBuilder::getHistohramChart

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilder::getLineChart() const
{
    return m_lineChart;

} // ChartBuilder::getLineChart

/*---------------------------------------------------------------------------*/
