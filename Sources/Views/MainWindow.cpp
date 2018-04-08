#include "Headers/Views/MainWindow.hpp"

#include "Headers/Views/CharBuilderDirector.hpp"

#include "ui_mainwindow.h"

/*---------------------------------------------------------------------------*/

MainWindow::MainWindow(
        QWidget *_parent
) : QWidget( _parent )
  , ui( new Ui::MainWindow )
  , m_controller ( nullptr )
  , m_setupColl( makeSetupCollectionImpl() )
{
    ui->setupUi( this );

} // MainWindow::MainWindow

/*---------------------------------------------------------------------------*/

MainWindow::~MainWindow()
{
  delete ui;

} // MainWindow::~MainWindow

/*---------------------------------------------------------------------------*/

void MainWindow::on_openFile_clicked()
{
   m_statFilePath = QFileDialog::getOpenFileName(
            this
        ,   tr("Открыть файл статистики")
        ,   QDir::currentPath()
        ,   tr("All Files (*.*)" )
   );

   ui->fileViewLineEdit->setText( m_statFilePath );

}

/*---------------------------------------------------------------------------*/

void
MainWindow::on_visualizeButton_clicked()
{
    if ( m_statFilePath.isEmpty() )
    {
        QMessageBox msgBox;
        msgBox.setText("Файл статистики не выбран!");
        msgBox.exec();
    }

    fillSetupData();
    /*-----------------------------------------------------------------------------------*/
    const QString inputFilePath{ QDir::currentPath() + "/Input_value_viol.inf" };
    m_controller = makeChartControllerImpl( inputFilePath.toStdString(), m_statFilePath.toStdString() );
    /*-----------------------------------------------------------------------------------*/
    ChartBuilderDirector director( m_controller->getModel() );

    auto histohramView = new QChartView( director.getHistogramChart() );
    auto chartView = new QChartView( director.getLineChart() );

    histohramView->setRenderHint( QPainter::Antialiasing );
    chartView->setRenderHint( QPainter::Antialiasing );
    ui->tabLayout->addWidget(histohramView );
    ui->tabLayout->addWidget( chartView );
}

void
MainWindow::fillSetupData()
{
    auto insert = [ & ]( const int _key, auto _value )
    {
        m_setupColl->insert( std::make_pair( _key, QVariant( _value ).toString() ) );
    };

    insert( IntegralSeparating, ui->integralSeparateSpinBox->value() );
    insert( SystemSize, ui->systemSizeSpinBox->value() );
    insert( RadiusMin, ui->radiusMinSpinBox->value() );
    insert( RadiusMax, ui->radiusMaxSpinBox->value() );
    insert( Depth, ui->depthSpinBox->value() );
    insert( DemfRatio, ui->demfRatioSpinBox->value() );
    insert( Epsilon, ui->epsilonSpinBox->value() );

    auto const & lisfreqSoundingList
          = ui->FreqSoundingTextEdit->toPlainText().split(",");
    auto const & regularAlfaValuesList
          = ui->regularAlfaValuesTextEdit->toPlainText().split(",");

    try
    {
        for( auto const& value: lisfreqSoundingList )
            boost::lexical_cast< double >( value.toStdString() );

    } catch ( boost::bad_lexical_cast ) {
        QMessageBox msgBox;
        msgBox.setText( "Частоты зондирования введены некорректно!" );
        msgBox.exec();

    }


}
