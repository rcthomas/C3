
#include "../C3_Context.hh"

// Execute application.

template< template< class > class Concurrency, class InstrumentTraits, template< class > class Engine >
inline int C3::Application< Concurrency, InstrumentTraits, Engine >::run( int& argc, char**& argv )
{
    using ContextType    = C3::Context< Concurrency, InstrumentTraits >;
    ContextType& context = ContextType::instance( argc, argv );
    Engine< ContextType > engine;
//  while( context.has_tasks() ) engine.process( context.next_task() );
    return context.finalize();
}

// Engine needs to get from context the information it needs to pick out its HDU to process.
// This is probably the frame name.
// With parallel concurrency this is determined at runtime from the MPI rank and the list of frames.
// With serial concurrency this is determined at runtime from input provided by the user.
// Since each concurrency depends on concurrency-related things at runtime it should be determined by that.
// And since that depends on the instrument traits, concurrency needs to know instrument traits (not necessarily inherit from it though).
