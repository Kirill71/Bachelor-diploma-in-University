#pragma once

/*---------------------------------------------------------------------------*/

#include "ph/ph.hpp"
#include "Model/PhysicalModel.hpp"
#include "Utils/Defines.hpp"

/*---------------------------------------------------------------------------*/

class MainWindow;


/*---------------------------------------------------------------------------*/

class ChartController
    :   public boost::noncopyable
{

/*---------------------------------------------------------------------------*/

    using PhisicalModelImplPtr = std::unique_ptr< PhysicalModel >;

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

    auto makePhysicalModel(
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
ChartController:: makePhysicalModel(
        const Defines::SetupCollPtr& _setup
    ,   const std::string& _inputStatFilePath
) const noexcept
{
    return std::make_unique< PhysicalModel >(
            _setup
        ,   _inputStatFilePath
    );

} // ChartController:: makePhysicalModel

/*---------------------------------------------------------------------------*/
