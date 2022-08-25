#pragma once

/*---------------------------------------------------------------------------*/

#include "ph/ph.hpp"
#include "Utils/Defines.hpp"

/*---------------------------------------------------------------------------*/

QT_CHARTS_USE_NAMESPACE

/*---------------------------------------------------------------------------*/

class Builder
    :   public boost::noncopyable
{
/*---------------------------------------------------------------------------*/

public:
/*---------------------------------------------------------------------------*/

    virtual ~Builder() noexcept {}

    virtual void build() = 0;

    virtual QChart* getHistohramChart( const Defines::Table& _table ) = 0;

    virtual QChart* getLineChart() const = 0;

    virtual QChart* geEchoSignalsChart() const = 0;

    virtual QChart* getDiscrepencyChart() const = 0;

/*---------------------------------------------------------------------------*/

}; // class Builder

/*---------------------------------------------------------------------------*/
