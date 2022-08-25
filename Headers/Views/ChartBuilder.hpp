#pragma once

/*---------------------------------------------------------------------------*/

#include "Views/Builder.hpp"
#include "Model/PhysicalModel.hpp"

/*---------------------------------------------------------------------------*/

class ChartBuilder
            :   public Builder
{
/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    explicit ChartBuilder(PhysicalModel& _model );

    ~ChartBuilder() override = default;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    void build() override;

    QChart* getHistohramChart( const Defines::Table& _table )  override;

    QChart* getLineChart() const override;

    QChart* geEchoSignalsChart() const override;

    QChart* getDiscrepencyChart() const override;

    void buildHistohramChart( const Defines::Table& _table );

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    void buildLineChart( const Defines::ChartData& _data );

    void buildEchoSignalsChart( const Defines::ChartData& _data );

    void buildDiscrepencyChart( const Defines::ChartData& _data );

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    QChart* m_histohramChart;

    QChart* m_lineChart;

    QChart* m_echoSignalsChart;

    QChart* m_discrepancyChart;

    PhysicalModel& m_model;

/*---------------------------------------------------------------------------*/

}; // class ChartBuilder

/*---------------------------------------------------------------------------*/
