#ifndef DEFINES_HPP
#define DEFINES_HPP

/*---------------------------------------------------------------------------*/

#include "Headers/ph/ph.hpp"
#include "Headers/Model/Gauss.hpp"

/*---------------------------------------------------------------------------*/

namespace Defines {

/*---------------------------------------------------------------------------*/

    using Table = std::vector<
        std::pair<
                double
              , double
         >
    >;

/*---------------------------------------------------------------------------*/

    using ChartData = std::tuple<
        Table
      , Gauss::Vector
      , Gauss::Vector
      , Gauss::Vector
      , Gauss::Vector
      , Gauss::Vector
      , Gauss::Vector
      , Gauss::Vector
    >;

/*---------------------------------------------------------------------------*/

    using SetupValuesCollImpl = std::map< const size_t, QString >;

    using SetupCollPtr = std::unique_ptr< SetupValuesCollImpl >;

/*---------------------------------------------------------------------------*/

} // namespace Defines

/*---------------------------------------------------------------------------*/

#endif // DEFINES_HPP
