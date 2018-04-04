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

    QChart* getHistogramChart() const;

    QChart* getLineChart() const;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    auto makeBuilderImplPtr( PhisicalModel& _model ) const;

    void construct();

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
