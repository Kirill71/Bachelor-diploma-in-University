#include "Headers/Controllers/ChartController.hpp"
#include "MainWindow.hpp"

ChartController::ChartController(
      MainWindow* _window
  ,   const std::string& _inputFilePath
  ,   const std::string& _inputStatFilePath
)
    :   m_window( _window )
    ,   m_model( makePhisicalModel( _inputFilePath, _inputStatFilePath ) )
{
}

