
#include "C3.hh"

// Runs a production application.

template< class Instrument, class Metadata, class Activity >
inline int C3::Application< Instrument, Metadata, Activity >::run( int& argc, char**& argv )
{
    C3::Environment< Instrument >&       environment = C3::Environment< Instrument >::instance( argc, argv );
    C3::Pipeline< Instrument, Activity > pipeline    = C3::Pipeline< Instrument, Operation >::create( argc, argv );
    C3::TaskQueue< Metadata >            queue       = C3::TaskQueue< Metadata >::create( argc, argv );
    while( queue.has_tasks() ) pipeline.process( queue.next_task() );
    return environment.finalize();
}
