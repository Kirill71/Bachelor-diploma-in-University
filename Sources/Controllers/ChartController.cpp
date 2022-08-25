#include "Controllers/ChartController.hpp"
#include "Views/MainWindow.hpp"
#include "Utils/Defines.hpp"

/*---------------------------------------------------------------------------*/

ChartController::ChartController(
        MainWindow* _window
    ,   const Defines::SetupCollPtr& _setup
    ,   const std::string& _inputStatFilePath
)
    :   m_window( _window )
    ,   m_model(makePhysicalModel(_setup, _inputStatFilePath) )
{
} // ChartController::ChartController

/*---------------------------------------------------------------------------*/

