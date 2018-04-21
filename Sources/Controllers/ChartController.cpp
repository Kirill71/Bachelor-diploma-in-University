#include "Headers/Controllers/ChartController.hpp"
#include "Headers/Views/MainWindow.hpp"
#include "Headers/Utils/Defines.hpp"

/*---------------------------------------------------------------------------*/

ChartController::ChartController(
        MainWindow* _window
    ,   const Defines::SetupCollPtr& _setup
    ,   const std::string& _inputStatFilePath
)
    :   m_window( _window )
    ,   m_model( makePhisicalModel( _setup, _inputStatFilePath ) )
{
} // ChartController::ChartController

/*---------------------------------------------------------------------------*/

