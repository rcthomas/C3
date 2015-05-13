#ifndef C3__APPLICATION
#define C3__APPLICATION

namespace C3
{

    /// @class Application
    /// @brief In production this should be all one needs to call from main().

    template< class Instrument, class Metadata, class Activity >
    struct Application
    {

        static int run( int& argc, char**& argv );

    };

}

#include "C3_Application.inl.hh"

#endif
