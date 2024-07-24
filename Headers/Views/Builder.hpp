#pragma once

/*---------------------------------------------------------------------------*/

#include "ph/ph.hpp"
#include "Utils/Defines.hpp"

/*---------------------------------------------------------------------------*/

class Builder
{
/*---------------------------------------------------------------------------*/

    Q_DISABLE_COPY_MOVE(Builder)

/*---------------------------------------------------------------------------*/

public:
/*---------------------------------------------------------------------------*/

    Builder() = default;

    virtual ~Builder() noexcept = default;

    virtual void build() = 0;

    virtual QChart* getHistohramChart( const Defines::Table& _table ) = 0;

    virtual QChart* getLineChart() const = 0;

    virtual QChart* geEchoSignalsChart() const = 0;

    virtual QChart* getDiscrepencyChart() const = 0;

/*---------------------------------------------------------------------------*/

}; // class Builder

/*---------------------------------------------------------------------------*/
