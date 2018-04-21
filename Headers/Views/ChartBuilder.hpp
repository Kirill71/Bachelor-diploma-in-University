#ifndef CHART_BUILDER_HPP_
#define CHART_BUILDER_HPP_

/*---------------------------------------------------------------------------*/

#include "Headers/Views/Builder.hpp"
#include "Headers/Model/PhisicalModel.hpp"
#include "Headers/Utils/Defines.hpp"

/*---------------------------------------------------------------------------*/

class ChartBuilder
            :   public Builder
{
/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    explicit ChartBuilder( PhisicalModel& _model );

    ~ChartBuilder() override = default;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    void build() override;

    QChart* getHistohramChart() const override;

    QChart* getLineChart() const override;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    void buildHistohramChart( const Defines::ChartData& _data );

    void buildLineChart( const Defines::ChartData& _data );

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    QChart* m_histohramChart;

    QChart* m_lineChart;

    PhisicalModel& m_model;

/*---------------------------------------------------------------------------*/

}; // class ChartBuilder

/*---------------------------------------------------------------------------*/

#endif
