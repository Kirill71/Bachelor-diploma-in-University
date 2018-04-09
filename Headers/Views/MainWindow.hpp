/*
    2018

    Author : Kirill Zharenkov, Aldec-KTC Team.

    Date creation: 01.03.2018

    Purpose: class for representation GUI.

    All right reserved (c).

*/

/*---------------------------------------------------------------------------*/

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

/*---------------------------------------------------------------------------*/

#include "Headers/ph/ph.hpp"

#include"Headers/Controllers/ChartController.hpp"

#include <QWidget>

/*---------------------------------------------------------------------------*/

namespace Ui {
  class MainWindow;
} // namespace Ui

/*---------------------------------------------------------------------------*/

class MainWindow : public QWidget
{

/*---------------------------------------------------------------------------*/

  Q_OBJECT

/*---------------------------------------------------------------------------*/

  using ChartControllerImplPtr = std::unique_ptr< ChartController >;

  using SetupValuesCollImpl = std::map< const int, QString >;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

  using SetupValuesCollImplPtr = std:: unique_ptr < SetupValuesCollImpl >;

/*---------------------------------------------------------------------------*/

public:

 /*---------------------------------------------------------------------------*/

  explicit MainWindow(
        QWidget* _parent = nullptr
  );

  ~MainWindow();

/*---------------------------------------------------------------------------*/

private slots:

/*---------------------------------------------------------------------------*/

    void on_openFile_clicked();

    void on_visualizeButton_clicked();

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  std::unique_ptr< ChartController >
  makeChartControllerImpl(
        const SetupValuesCollImplPtr& _setup
    ,   const std::string& _inputStatFilePath
  ) const noexcept;

  auto makeSetupCollectionImpl() const noexcept;

  void fillSetupData();

  void getMessageBox( const QString& _text ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  Ui::MainWindow *ui;

  ChartControllerImplPtr m_controller;

  SetupValuesCollImplPtr m_setupColl;

  QString m_statFilePath;

/*---------------------------------------------------------------------------*/

}; // class MainWindow

/*---------------------------------------------------------------------------*/

inline std::unique_ptr< ChartController >
MainWindow::makeChartControllerImpl(
      const SetupValuesCollImplPtr& _setup
    , const std::string& _inputStatFilePath
 ) const noexcept
{

    return std::make_unique< ChartController >(
            const_cast< MainWindow* >( this )
       ,    _setup
       ,    _inputStatFilePath
    );

} // MainWindow::makePhisicalModelImpl

/*---------------------------------------------------------------------------*/

inline auto
MainWindow::makeSetupCollectionImpl() const noexcept
{
    return std::make_unique< SetupValuesCollImpl >();

} // MainWindow::makeSetupCollectionImpl

/*---------------------------------------------------------------------------*/

#endif // MAINWINDOW_HPP
