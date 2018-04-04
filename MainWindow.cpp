#include "MainWindow.hpp"

#include "Headers/Views/CharBuilderDirector.hpp"

#include "ui_mainwindow.h"

/*---------------------------------------------------------------------------*/

MainWindow::MainWindow(
        QWidget *_parent
    ,   const std::string& _inputFilePath
    ,   const std::string& _inputStatFilePath
) : QWidget( _parent )
  , ui( new Ui::MainWindow )
  , m_controller( makeChartControllerImpl( _inputFilePath, _inputStatFilePath ) )
{
    ui->setupUi( this );

    ChartBuilderDirector director( m_controller->getModel() );

    auto histohramView = new QChartView( director.getHistogramChart() );
    auto chartView = new QChartView( director.getLineChart() );
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
