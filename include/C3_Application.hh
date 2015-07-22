#ifndef C3_APPLICATION_HH
#define C3_APPLICATION_HH

namespace C3
{

    /// @class Application
    /// @brief In production this should be all one needs to call from main().

    template< template< class > class Concurrency, class InstrumentTraits, template< class > class ProcessingEngine >
    struct Application
    {

        /// Execute application, return an exit code.
        static int run( int& argc, char**& argv );

    };

}

#include "inline/C3_Application.hh"

#endif
