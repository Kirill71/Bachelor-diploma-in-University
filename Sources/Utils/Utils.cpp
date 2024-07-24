#include "Utils/Utils.hpp"

/*-----------------------------------------------------------------*/

namespace Utils{

/*-----------------------------------------------------------------*/

    void makeThread( const std::function< void() >& _functor )
    {
        auto numThreads = std::thread::hardware_concurrency();
        if ( numThreads > SINGLE_THREAD_MODE )
        {
            std::thread thread( _functor );
            if ( thread.joinable() )
                thread.join();
        }
        else
        {
            _functor();
        }
    }

/*-----------------------------------------------------------------*/

} // namespace Utils
