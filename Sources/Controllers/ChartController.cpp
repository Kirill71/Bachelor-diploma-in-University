#include "Headers/Controllers/ChartController.hpp"
#include "Headers/Views/MainWindow.hpp"

/*---------------------------------------------------------------------------*/

ChartController::ChartController(
        MainWindow* _window
    ,   const SetupCollPtr& _setup
    ,   const std::string& _inputStatFilePath
)
    :   m_window( _window )
    ,   m_model( makePhisicalModel( _setup, _inputStatFilePath ) )
{
} // ChartController::ChartController

/*---------------------------------------------------------------------------*/

