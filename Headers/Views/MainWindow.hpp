#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
/*---------------------------------------------------------------------------*/

#include"Headers/ph/ph.hpp"
#include <QMainWindow>
#include "Headers/Model/PhisicalModel.hpp"

/*---------------------------------------------------------------------------*/

namespace Ui {
  class MainWindow;

} // Ui

/*---------------------------------------------------------------------------*/

class MainWindow : public QMainWindow
{

 /*---------------------------------------------------------------------------*/

  Q_OBJECT

/*---------------------------------------------------------------------------*/

  using PhisicalModelPtr = std::unique_ptr< PhisicalModel >;

 /*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

  MainWindow(
        QWidget *parent = nullptr
      , const std::string & _filePath = std::string()
  );

  ~MainWindow() noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  auto makeModelImpl( const std::string& _filePath ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

  Ui::MainWindow *ui;

  PhisicalModelPtr m_model;

}; // class MainWindow

/*---------------------------------------------------------------------------*/

inline auto
MainWindow::makeModelImpl(const std::string& _filePath ) const noexcept
{
  return std::make_unique< PhisicalModel >( _filePath );

} // MainWindow::makeModelImpl

/*---------------------------------------------------------------------------*/

#endif // MAINWINDOW_HPP

/*---------------------------------------------------------------------------*/
