#include "Headers/Views/MainWindow.hpp"
#include "Headers/Views/CharBuilderDirector.hpp"
#include "Headers/Views/NumbersDelegate.hpp"

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

    ui->staticTable->horizontalHeader()
            ->setSectionResizeMode( QHeaderView::Stretch );

    ui->staticTable->setItemDelegate( new NumbersDelegate );
} // MainWindow::MainWindow

/*---------------------------------------------------------------------------*/

MainWindow::~MainWindow()
{
  delete ui;

} // MainWindow::~MainWindow

/*---------------------------------------------------------------------------*/

void
MainWindow::on_openFile_clicked()
{
   m_statFilePath = QFileDialog::getOpenFileName(
            this
        ,   tr("Открыть файл статистики")
        ,   QDir::currentPath()
        ,   tr("All Files (*.stat)" )
   );

   if (m_statFilePath.isEmpty())
   {
       return;
   }

   if ( ui->horizontalLayout_19->count() )
   {
      m_setupColl->clear();
      delete ui->tabLayout->takeAt( 0 );
   }
   ui->fileViewLineEdit->setText( m_statFilePath );
   Defines::Table table;
   drawTable( table );

   m_controller = makeChartControllerImpl( m_setupColl, m_statFilePath.toStdString() );
   ChartBuilderDirector director( m_controller->getModel() );
   auto histohramView = new QChartView( director.getHistogramChart( table ) );
   histohramView->setRenderHint( QPainter::Antialiasing );
   ui->horizontalLayout_19->addWidget( histohramView );

} // MainWindow::on_openFile_clicked

/*---------------------------------------------------------------------------*/

void
MainWindow::drawTable(  Defines::Table& _table ) const
{
    std::ifstream statFile( m_statFilePath.toLocal8Bit().constData() );

    if ( !statFile.is_open() )
        std::logic_error(" Файл статистики не был открыт, возможно, он не выбран!!!");

    while( !statFile.eof() )
    {
      double first, second;
      statFile >> first >> second;
      _table.push_back( std::make_pair( first , second ) ) ;
      ui->staticTable->insertRow( ui->staticTable->rowCount() );
      ui->staticTable->setItem(ui->staticTable->rowCount()-1, 0, new QTableWidgetItem( std::to_string( first ).c_str() ) );
      ui->staticTable->setItem( ui->staticTable->rowCount()-1, 1, new QTableWidgetItem( std::to_string( second ).c_str()  ) );
    };

    _table.erase( --_table.end() );
    ui->staticTable->removeRow( ui->staticTable->rowCount()-1 );

    statFile.close();
}

/*---------------------------------------------------------------------------*/

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

    auto const& freqSoundingList
          = ui->FreqSoundingTextEdit->toPlainText().split(",");
    auto const& regularAlfaValuesList
          = ui->regularAlfaValuesTextEdit->toPlainText().split(",");

    int index = ui->indexAlfaSpinBox->value();
    if ( index > regularAlfaValuesList.size() )
    {
        getMessageBox("Порядковый номер альфа вышел за границы диапазона!");
        return;
    }
    insert( SoundingFreqNumber, freqSoundingList.size() );
    insert( AlfaNumber, index );
    insert( RegularAlfaValue, ui->regularAlfaValuesTextEdit->toPlainText() );

    try
    {
        for( auto const& value: freqSoundingList )
            boost::lexical_cast< double >( value.toStdString() );

    } catch ( boost::bad_lexical_cast )
    {
       getMessageBox("Частоты зондирования введены некорректно!");
       return;

    }

    insert(SoundingFrequensies, ui->FreqSoundingTextEdit->toPlainText() );

} //MainWindow::fillSetupData

/*---------------------------------------------------------------------------*/

void
MainWindow::getMessageBox(const QString& _text ) const noexcept
{
    QMessageBox msgBox;
    msgBox.setText( _text );
    msgBox.exec();

} // MainWindow::getMessageBox

/*---------------------------------------------------------------------------*/

void
MainWindow::on_visualizeButton_clicked()
{

    if ( m_statFilePath.isEmpty() )
    {
       getMessageBox("Файл статистики не выбран!");
       return;
    }
    if ( ui->tabLayout->count() )
    {
       m_setupColl->clear();
       delete ui->tabLayout->takeAt( 0 );
       delete ui->tabLayout->takeAt( 0 );
    }

    fillSetupData();
    m_controller = makeChartControllerImpl( m_setupColl, m_statFilePath.toStdString() );
    ChartBuilderDirector director( m_controller->getModel() );
    director.construct();

    auto chartView = new QChartView( director.getLineChart() );
    auto  echoSignalsView = new QChartView( director.getEchoSignalsChart() );
    auto discrepencyView = new QChartView( director.getDiscrepencyChart() );

    chartView->setRenderHint( QPainter::Antialiasing );
    echoSignalsView->setRenderHint( QPainter::Antialiasing );


    QLayout* vertical = new QVBoxLayout();
    vertical->addWidget( echoSignalsView );
    vertical->addWidget( discrepencyView );

    ui->tabLayout->addLayout( vertical );
    ui->tabLayout->addWidget( chartView );


} // MainWindow::on_visualizeButton_clicked

/*---------------------------------------------------------------------------*/
