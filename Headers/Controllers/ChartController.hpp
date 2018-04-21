#ifndef CHART_CONTROLLER_HPP_
#define CHART_CONTROLLER_HPP_

#include "Headers/ph/ph.hpp"
#include "Headers/Model/PhisicalModel.hpp"
#include "Headers/Utils/Defines.hpp"

/*---------------------------------------------------------------------------*/

class MainWindow;


/*---------------------------------------------------------------------------*/

class ChartController
    :   public boost::noncopyable
{

/*---------------------------------------------------------------------------*/

    using PhisicalModelImplPtr = std::unique_ptr< PhisicalModel >;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    ChartController(
           MainWindow* _window
       ,   const Defines::SetupCollPtr& _setup
       ,   const std::string& _inputStatFilePath
    );

    auto& getModel() const noexcept { return *m_model; }


/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    auto makePhisicalModel(
            const Defines::SetupCollPtr& _setup
        ,   const std::string& _inputStatFilePath
    ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    MainWindow* m_window;

    PhisicalModelImplPtr m_model;

/*---------------------------------------------------------------------------*/

}; // class ChartController

/*---------------------------------------------------------------------------*/

inline auto
ChartController:: makePhisicalModel(
        const Defines::SetupCollPtr& _setup
    ,   const std::string& _inputStatFilePath
) const noexcept
{
    return std::make_unique< PhisicalModel >(
            _setup
        ,   _inputStatFilePath
    );

} // ChartController:: makePhisicalModel

/*---------------------------------------------------------------------------*/

#endif // !CHART_CONTROLLER_HPP_
