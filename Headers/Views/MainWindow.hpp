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

 /*---------------------------------------------------------------------------*/

public:

 /*---------------------------------------------------------------------------*/

  explicit MainWindow(
        QWidget* _parent = nullptr
    ,   const std::string& _inputFilePath = std::string()
    ,   const std::string& _filePath = std::string()
  );

  ~MainWindow();

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  auto makeChartControllerImpl(
        const std::string& _inputFilePath
    ,   const std::string& _inputStatFilePath
  ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  Ui::MainWindow *ui;

  ChartControllerImplPtr m_controller;

/*---------------------------------------------------------------------------*/

}; // class MainWindow

/*---------------------------------------------------------------------------*/

inline auto
MainWindow::makeChartControllerImpl(
      const std::string& _inputFilePath
    , const std::string& _inputStatFilePath
 ) const noexcept
{

    return new ChartController(
            const_cast< MainWindow* >( this )
       ,    _inputFilePath
       ,    _inputStatFilePath
    );

} // MainWindow::makePhisicalModelImpl

/*---------------------------------------------------------------------------*/

#endif // MAINWINDOW_HPP
