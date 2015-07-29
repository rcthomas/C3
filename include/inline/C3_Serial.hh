
#include <fstream>

#include "../C3_FitsCreator.hh"
#include "../C3_FitsLoader.hh"
#include "../C3_Frame.hh"

// Configuration and task stream from command line.

template< class InstrumentTraits >
inline void C3::Serial< InstrumentTraits >::init( int& argc, char**& argv )
{

    // Read and validate configuration.

    std::ifstream stream( argv[ 1 ] );
    if( ! stream ) throw 123;   // FIXME actual exception
    _config = YAML::Load( stream );

    if( ! InstrumentTraits::frame_exists( _config[ "frame" ].template as< std::string >() ) ) throw 124;    // FIXME actual exception

    // Parse other arguments into list of task files.

    for( auto i = 2; i < argc; ++ i ) _task_files.push( argv[ i ] );

}

// Grab next task from stream.

template< class InstrumentTraits >
inline YAML::Node C3::Serial< InstrumentTraits >::next_task() 
{ 

    if( _tasks.size() == 0 && _task_files.size() > 0 )
    {
        std::string task_file = _task_files.front();
        _task_files.pop();
        std::ifstream stream( task_file.c_str() );
        if( ! stream ) throw 123;   // FIXME actual exception
        std::vector< YAML::Node > tasks = YAML::LoadAll( stream );
        for( auto& task : tasks ) _tasks.push( std::move( task ) );
    }

    YAML::Node task = _tasks.front(); 
    _tasks.pop();
    return task;

}

// Serial is easy case, just load my frame!

template< class InstrumentTraits >
template< class T >
inline void C3::Serial< InstrumentTraits >::load( C3::Frame< T >& frame, const std::string& path )
{
    C3::FitsLoader loader( path );
    loader( frame.block(), this->frame() );
}

// Serial is easy, just save my damn stuff!

template< class InstrumentTraits >
template< class T >
inline void C3::Serial< InstrumentTraits >::save( C3::Frame< T >& frame, const std::string& path )
{
    C3::FitsCreator creator( path );
    std::vector< C3::size_type > shape( 2 );
    shape[ 0 ] = frame.ncolumns();
    shape[ 1 ] = frame.nrows();
    creator( frame.block(), this->frame(), shape );
}

// Serial is easy, just save my damn stuff!

template< class InstrumentTraits >
template< class T, class U >
inline void C3::Serial< InstrumentTraits >::save( C3::Frame< T >& output, C3::Frame< T >& invvar, C3::Frame< U >& flags, const std::string& path )
{
    C3::FitsCreator creator( path );
    std::vector< C3::size_type > shape( 2 );
    shape[ 0 ] = output.ncolumns();
    shape[ 1 ] = output.nrows();
    creator( output.block(), this->frame(), shape );
    creator( invvar.block(), this->frame() + "_INVVAR", shape );
    creator(  flags.block(), this->frame() + "_FLAGS" , shape );
}
