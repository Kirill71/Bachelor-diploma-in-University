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

   ui->fileViewLineEdit->setText( m_statFilePath );

   drawTable();

} // MainWindow::on_openFile_clicked

/*---------------------------------------------------------------------------*/

void
MainWindow::drawTable() const
{
    std::ifstream statFile( m_statFilePath.toLocal8Bit().constData() );

    if ( !statFile.is_open() )
        std::logic_error(" Трощило ты заебал, файл сука cоздай/выбери !!!");


    while( !statFile.eof() )
    {
      std::string first, second;
      statFile >> first >> second;

      ui->staticTable->insertRow( ui->staticTable->rowCount() );
      ui->staticTable->setItem(ui->staticTable->rowCount()-1, 0, new QTableWidgetItem( first.c_str() ) );
      ui->staticTable->setItem( ui->staticTable->rowCount()-1, 1, new QTableWidgetItem( second.c_str() ) );
    };

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

    auto histohramView = new QChartView( director.getHistogramChart() );
    auto chartView = new QChartView( director.getLineChart() );

    histohramView->setRenderHint( QPainter::Antialiasing );
    chartView->setRenderHint( QPainter::Antialiasing );
    ui->tabLayout->addWidget(histohramView );
    ui->tabLayout->addWidget( chartView );

} // MainWindow::on_visualizeButton_clicked

/*---------------------------------------------------------------------------*/
