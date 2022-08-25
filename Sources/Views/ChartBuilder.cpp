
#include "Views/ChartBuilder.hpp"
#include "Utils/Utils.hpp"



namespace

{

template<typename AxiesType = QBarCategoryAxis>
struct ChartAxies final
{
    AxiesType* x;
    AxiesType* y;
};

template<typename ChartType, typename AxiesType>
void setAxies(ChartType* chart, ChartAxies<AxiesType>& axies)
{
    chart->createDefaultAxes();
    chart->addAxis( axies.x, Qt::AlignBottom );
    chart->addAxis( axies.y,  Qt::AlignLeft );
}

} // anon namespace

/*---------------------------------------------------------------------------*/

ChartBuilder::ChartBuilder( PhisicalModel& _model  )
    :   m_histohramChart( new QChart() )
    ,   m_lineChart( new QChart() )
    ,   m_echoSignalsChart( new QChart() )
    ,   m_discrepancyChart( new QChart() )
    ,   m_model( _model )
{
} // ChartBuilder::ChartBuilder

/*---------------------------------------------------------------------------*/

void
ChartBuilder::build()
{
    auto data = m_model.calculatePhisicalModel();

    buildLineChart( data );
    buildEchoSignalsChart( data );
    buildDiscrepencyChart( data );

} // ChartBuilder::build

/*---------------------------------------------------------------------------*/

void
ChartBuilder::buildHistohramChart( const Defines::Table& _table )
{

   std::vector< QBarSet* > sets;
   auto histSeries = new QBarSeries;
   for ( auto const & row : _table )
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
                 _table.cbegin()
            ,    _table.cend()
            ,    []( auto _lhs, auto _rhs )
                 {
                     return _lhs.second < _rhs.second;
                 }
           )->second
   );

   axisY->setTickCount( 7 );
   m_histohramChart->addAxis(axisY, Qt::AlignLeft);
   histSeries->attachAxis(axisY);
   m_histohramChart->legend()->setVisible( true );
   m_histohramChart->legend()->setAlignment( Qt::AlignBottom );

   m_histohramChart->setAnimationOptions( QChart::SeriesAnimations );

} // ChartBuilder::buildHistohramChart

/*---------------------------------------------------------------------------*/

void
ChartBuilder::buildLineChart( const Defines::ChartData& _data )
{
    auto radiusCollection = std::get< 1 >( _data );
    auto nmCollection = std::get< 2 >( _data );
    auto nrCollection = std::get< 3 > ( _data );

    QLineSeries* series = new QLineSeries();

    auto lineChartSetter = [ & ]( const auto& _collection, const char* _title )
    {
        for ( size_t i = 0; i < _collection.size(); ++i )
        {
          *series << QPointF( radiusCollection[ i ], _collection[ i ] );
           series->setName( QString::fromLocal8Bit( _title ) );
        }
    };

#ifdef _WIN32
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "Windows-1251") );
#endif

    lineChartSetter( nmCollection,"прямая задача");

    m_lineChart->addSeries( series );

    series = new QLineSeries();
    lineChartSetter( nrCollection, "обратная задача" );

    m_lineChart->addSeries( series );

    m_lineChart->setAnimationOptions( QChart::SeriesAnimations );

    auto axisX = new QValueAxis();
    axisX->setMin(0.0);
    auto axisY = new QValueAxis();
    axisY->setMin(0.0);

    ChartAxies<QValueAxis> axies {axisX, axisY};
    setAxies(m_lineChart, axies);

    m_lineChart->legend()->setVisible( true );
    m_lineChart->legend()->setAlignment( Qt::AlignBottom );
    m_lineChart->legend()->attachToChart();

} // ChartBuilder::buildLineChart

/*---------------------------------------------------------------------------*/

void
ChartBuilder::buildEchoSignalsChart( const Defines::ChartData& _data )
{
    auto mv_f = std::get< 4 >( _data );
    auto freqExp = std::get< 5 >( _data );

    QLineSeries* series = new QLineSeries();
    QStringList axisYSeries;

#ifdef _WIN32
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "Windows-1251") );
#endif

    const size_t size = mv_f.size();
    for ( size_t i = 0; i < size; ++i )
    {
        *series << QPointF( freqExp[ i ], mv_f[ i ] );
        series->setName( QString::fromLocal8Bit( "Анализ эхо - сигналов." ) );
        axisYSeries.push_back( std::to_string( mv_f[ size - i - 1 ] ).c_str() );
    }

    m_echoSignalsChart->addSeries( series );
    auto axisX = new QBarCategoryAxis();
    auto axisY = new QBarCategoryAxis();

    auto& settings = m_model.getSetupData();
    axisX->append( settings[ SoundingFrequensies ].split( "," ) );
    axisY->append( axisYSeries );

    ChartAxies<> axies{axisX, axisY};
    setAxies(m_echoSignalsChart, axies);

    m_echoSignalsChart->setAnimationOptions( QChart::SeriesAnimations );

    m_echoSignalsChart->legend()->setVisible( true );
    m_echoSignalsChart->legend()->setAlignment( Qt::AlignBottom );
    m_echoSignalsChart->legend()->attachToChart();

} // ChartBuilder::buildEchoSignalsChart

/*---------------------------------------------------------------------------*/


void
ChartBuilder::buildDiscrepencyChart( const Defines::ChartData& _data )
{
    auto delta_al = std::get< 6 >( _data );
    auto alphaVector = std::get< 7 >( _data );

    QLineSeries* series = new QLineSeries();
    QStringList axisYSeries;

#ifdef _WIN32
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "Windows-1251") );
#endif

    const size_t size = delta_al.size();
    for ( size_t i = 0; i < size; ++i )
    {
        *series << QPointF( alphaVector[ i ], delta_al[ i ] );
        series->setName( QString::fromLocal8Bit( "Невязка данных по альфа." ) );
        axisYSeries.push_back( std::to_string( delta_al[ size - i - 1 ] ).c_str() );
    }

    m_discrepancyChart->addSeries( series );
    auto axisX = new QBarCategoryAxis();
    auto axisY = new QBarCategoryAxis();

    auto& settings = m_model.getSetupData();
    axisX->append( settings[ RegularAlfaValue ].split( "," ) );
    axisY->append( axisYSeries );

    ChartAxies<> axies{axisX, axisY};
    setAxies(m_discrepancyChart, axies);

    m_discrepancyChart->setAnimationOptions( QChart::SeriesAnimations );

    m_discrepancyChart->legend()->setVisible( true );
    m_discrepancyChart->legend()->setAlignment( Qt::AlignBottom );
    m_discrepancyChart->legend()->attachToChart();

} // ChartBuilder::buildDescrepencyChart

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilder::getHistohramChart( const Defines::Table& _table )
{

    buildHistohramChart( _table );
    return m_histohramChart;

} // ChartBuilder::getHistohramChart

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilder::getLineChart() const
{
    return m_lineChart;

} // ChartBuilder::getLineChart

/*---------------------------------------------------------------------------*/

QChart*
ChartBuilder::geEchoSignalsChart() const
{
    return m_echoSignalsChart;

} // ChartBuilder::geEchoSignalsChart


/*---------------------------------------------------------------------------*/


QChart*
ChartBuilder::getDiscrepencyChart() const
{
    return m_discrepancyChart;

} // ChartBuilder::getDescrepencyChart


/*---------------------------------------------------------------------------*/

