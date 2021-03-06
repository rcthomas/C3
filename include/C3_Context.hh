#ifndef C3_CONTEXT_HH
#define C3_CONTEXT_HH

#include <yaml-cpp/yaml.h>

namespace C3
{

    /// @class Context
    /// @brief Processing context (parallel, serial, and so on).
    ///
    /// Context hosts a concurrency policy (something like serial or parallel
    /// that determines how execution is going to be done).

    template< template< class > class Concurrency, class InstrumentTraits >
    class Context : public Concurrency< InstrumentTraits >
    {

        public :    // Public methods.

            /// Initialization access point.
            static Context& instance( int& argc, char**& argv );

            /// Access point after initialization.
            static Context& instance();

            /// No copy constructor.
            Context( const Context& context ) = delete;

            /// No copy assignment operator.
            Context& operator = ( const Context& context ) = delete;

        private :   // Private methods.

            /// Constructor.
            Context() = default;

    };

}

#include "inline/C3_Context.hh"

#endif
