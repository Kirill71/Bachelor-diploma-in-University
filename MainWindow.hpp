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
#include "Headers/Model/PhisicalModel.hpp"

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

  using PhisicalModelImplPtr = std::unique_ptr< PhisicalModel >;

 /*---------------------------------------------------------------------------*/

public:

 /*---------------------------------------------------------------------------*/

  explicit MainWindow(
        QWidget *_parent = nullptr
    ,   const std::string& _inputFilePath = std::string()
    ,   const std::string& _filePath = std::string()
  );

  ~MainWindow() noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  auto makePhisicalModelImpl(
        const std::string& _inputFilePath
      , const std::string& _inputStatFilePath
  ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  Ui::MainWindow *ui;

  PhisicalModelImplPtr m_model;

/*---------------------------------------------------------------------------*/

}; // class MainWindow

/*---------------------------------------------------------------------------*/

inline auto
MainWindow::makePhisicalModelImpl(
      const std::string& _inputFilePath
    , const std::string& _inputStatFilePath
 ) const noexcept
{
    return std::make_unique< PhisicalModel > ( _inputFilePath, _inputStatFilePath );

} // MainWindow::makePhisicalModelImpl

/*---------------------------------------------------------------------------*/

#endif // MAINWINDOW_HPP
