#ifndef CHARBUILDERDIRECTOR_HPP
#define CHARBUILDERDIRECTOR_HPP

/*---------------------------------------------------------------------------*/

#include "Headers/Views/Builder.hpp"
#include "Headers/Views/ChartBuilder.hpp"

/*---------------------------------------------------------------------------*/

class ChartBuilderDirector
{

/*---------------------------------------------------------------------------*/

    using BuilderImplPtr = std::unique_ptr< Builder >;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

    explicit ChartBuilderDirector( PhisicalModel& _model );

    QChart* getHistogramChart( const Defines::Table& _table ) const;

    QChart* getLineChart() const;

    QChart* getEchoSignalsChart() const;

    QChart* getDiscrepencyChart() const;

    void construct();

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    auto makeBuilderImplPtr( PhisicalModel& _model ) const;

/*---------------------------------------------------------------------------*/

    BuilderImplPtr m_builder;

/*---------------------------------------------------------------------------*/

}; // class ChartBuilderDirector

/*---------------------------------------------------------------------------*/

inline auto
ChartBuilderDirector::makeBuilderImplPtr( PhisicalModel& _model ) const
{
    return std::make_unique< ChartBuilder >( _model );
}

#endif // CHARBUILDERDIRECTOR_HPP
