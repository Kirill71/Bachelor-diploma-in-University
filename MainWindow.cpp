#include "MainWindow.hpp"

#include "ui_mainwindow.h"

/*---------------------------------------------------------------------------*/

MainWindow::MainWindow(
        QWidget *_parent
    ,   const std::string& _inputFilePath
    ,   const std::string& _inputStatFilePath
) : QWidget( _parent )
  , ui( new Ui::MainWindow )
  , m_model( makePhisicalModelImpl( _inputFilePath, _inputStatFilePath ) )
{

    ui->setupUi( this );

    auto chartData = m_model->calculatePhisicalModel();
    auto radiusCollection = std::get< 1 >( chartData );
    auto nmCollection = std::get< 2 >( chartData );
    auto nrCollection = std::get< 3 > ( chartData );

    std::vector< QBarSet* > sets;

    auto histSeries = new QBarSeries;
    for (auto const & row : table )
    {
        sets.push_back( new QBarSet( std::to_string( row.first ).substr(0,3).c_str() ) );
        *sets.back() << row.second;
        histSeries->append( sets.back() );
    }

    QLineSeries *series = new QLineSeries();
    for ( size_t i = 0; i< nmCollection.size(); ++i )
    {
        *series << QPointF( radiusCollection[ i ], nmCollection[ i ] );
        series->setName("input chart");
    }
    QChart *histogram = new QChart();
    histogram->addSeries( histSeries );

    QChart *chart = new QChart();
    chart->addSeries(series);

    series = new QLineSeries();
    for ( size_t i = 0; i< nrCollection.size(); ++i )
    {
        *series << QPointF(radiusCollection[ i ], nrCollection[ i ] );
        series->setName("output chart");
    }
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
    histogram->setAxisY( axisY, histSeries);
    histogram->legend()->setVisible(true);
    histogram->legend()->setAlignment( Qt::AlignBottom );

    histogram->setAnimationOptions( QChart::SeriesAnimations );
    chart->setAnimationOptions( QChart::SeriesAnimations );

   chart->addSeries(series);
   chart->createDefaultAxes();
   chart->axisX()->setMin(0.0);
   chart->axisX()->setMax(*std::max_element( radiusCollection.begin(), radiusCollection.end() ) );
   chart->axisY()->setMin(0.0);
   chart->axisY()->setMax(
        std::max(
                *std::max_element(
                        nmCollection.begin()
                    ,   nmCollection.end()
                )
             ,  *std::max_element(
                        nrCollection.begin()
                    ,   nrCollection.end()
               )
       ) + 0.015
   );

    chart->legend()->setVisible( true );
    chart->legend()->setAlignment( Qt::AlignBottom );
    chart->legend()->attachToChart();

    auto histohramView = new QChartView( histogram );
    auto chartView = new QChartView( chart );
    histohramView->setRenderHint( QPainter::Antialiasing );
    chartView->setRenderHint( QPainter::Antialiasing );
    ui->tabLayout->addWidget(histohramView );
    ui->tabLayout->addWidget( chartView );

} // MainWindow::MainWindow

/*---------------------------------------------------------------------------*/

MainWindow::~MainWindow()
{
  delete ui;

} // MainWindow::~MainWindow

/*---------------------------------------------------------------------------*/
