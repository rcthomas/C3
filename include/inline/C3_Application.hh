
#include "../C3.hh"
#include "../C3_Context.hh"
#include "../C3_Logger.hh"

// Execute application.

template< template< class > class Concurrency, class InstrumentTraits, template< class > class Engine >
inline int C3::Application< Concurrency, InstrumentTraits, Engine >::run( int& argc, char**& argv )
{

    // Context singleton.

    using ContextType = C3::Context< Concurrency, InstrumentTraits >;
    ContextType& context = ContextType::instance( argc, argv );

    // Launch preprocessing engine and feed it task documents.

    {

        C3::Logger& logger = context.logger();

        // Preprocessing engine.

        logger.info( "Launching preprocessing engine for frame:", context.frame() );
        Engine< ContextType > engine;

        // Engine handles each task passed to it by the context.

        C3::size_type counter = 0;
        while( context.has_tasks() ) 
        {
            logger.info( "Starting next preprocessing task. " );
            engine.process( context.next_task() );
            logger.info( "Preprocessing task complete. Total tasks completed so far:", ++ counter );
        }

        logger.info( "Shutting down preprocessing engine. Total tasks completed:", counter );

    }

    // Shutdown.

    return context.finalize();

}

// Engine needs to get from context the information it needs to pick out its HDU to process.
// This is probably the frame name.
// With parallel concurrency this is determined at runtime from the MPI rank and the list of frames.
// With serial concurrency this is determined at runtime from input provided by the user.
// Since each concurrency depends on concurrency-related things at runtime it should be determined by that.
// And since that depends on the instrument traits, concurrency needs to know instrument traits (not necessarily inherit from it though).
