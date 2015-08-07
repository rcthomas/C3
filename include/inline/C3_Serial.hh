
#include <fstream>

#include "../C3_Exception.hh"
#include "../C3_FileLogger.hh"
#include "../C3_FitsCreator.hh"
#include "../C3_FitsLoader.hh"
#include "../C3_Frame.hh"
#include "../C3_StandardLogger.hh"

// Initialize command line, config, validation, tasks, etc.

template< class InstrumentTraits >
inline void C3::Serial< InstrumentTraits >::init( int& argc, char**& argv )
{

    // Initialization in steps mostly for readability.

    _validate_command_line( argc, argv );
    _init_config( argc, argv );
    _validate_frame();
    _init_logger();
    _init_task_queue( argc, argv );

    logger().info( "Serial context initialization complete." );

}

// Executed on exit.

template< class InstrumentTraits >
inline int C3::Serial< InstrumentTraits >::finalize() 
{ 
    logger().info( "Serial context finalizing." );
    logger().info( "Goodbye!" );
    return EXIT_SUCCESS; 
}

// Grab next task from stream.  If task queue is exhausted but task files
// remain, replenish queue with tasks from next task file.

template< class InstrumentTraits >
inline YAML::Node C3::Serial< InstrumentTraits >::next_task() 
{ 

    if( _tasks.size() == 0 && _task_files.size() > 0 )
    {

        std::string task_file = _task_files.front();
        _task_files.pop();
    
        logger().debug( "Enqueuing tasks from from:", task_file );

        std::ifstream stream( task_file );
        if( ! stream ) throw C3::Exception::create( "Can't open task file:", task_file );

        std::vector< YAML::Node > tasks = YAML::LoadAll( stream );
        for( auto& task : tasks ) _tasks.push( std::move( task ) );

    }

    logger().debug( "Next task. Tasks currently in queue:", _tasks.size() );

    YAML::Node task = _tasks.front(); 
    _tasks.pop();
    return task;

}

// Load frame.

template< class InstrumentTraits >
template< class T >
inline void C3::Serial< InstrumentTraits >::load( C3::Frame< T >& input, const std::string& path )
{

    logger().debug( "Loading frame", frame(), "from", path, "[START]" );

    C3::FitsLoader loader( path );
    loader.load( input.block(), frame() );

    logger().debug( "Loading frame", frame(), "from", path, "[DONE]" );

}

// Save unconverted frame.

template< class InstrumentTraits >
template< class T >
inline void C3::Serial< InstrumentTraits >::save( C3::Frame< T >& output, const std::string& path )
{
    save< T, T >( output, path );
}

// Save converted frame.

template< class InstrumentTraits >
template< class T, class U >
inline void C3::Serial< InstrumentTraits >::save( C3::Frame< U >& output, const std::string& path )
{

    logger().debug( "Saving frame", frame(), "to", path, "[START]" );

    C3::FitsCreator creator( path );

    int  naxis = 2;
    long naxes[ 2 ] { output.ncolumns(), output.nrows() };
    creator.create< T, U >( output.block(), frame(), naxis, naxes );

    logger().debug( "Saving frame", frame(), "to", path, "[DONE]" );
    logger().debug( "Output", output.block().size(), "pixels,", output.ncolumns(), "columns x", output.nrows() );
}

// Save frame tuple without conversion of output and inverse variance.

template< class InstrumentTraits >
template< class T, class U >
inline void C3::Serial< InstrumentTraits >::save( C3::Frame< T >& output, C3::Frame< T >& invvar, C3::Frame< U >& flags, const std::string& path )
{
    save< T, T, U >( output, invvar, flags, path );
}

// Save frame tuple with conversion of output and inverse variance.

template< class InstrumentTraits >
template< class T, class U, class V >
inline void C3::Serial< InstrumentTraits >::save( C3::Frame< U >& output, C3::Frame< U >& invvar, C3::Frame< V >& flags, const std::string& path )
{

    logger().debug( "Saving frame tuple", frame(), "to", path, "[START]" );

    C3::FitsCreator creator( path );

    int  naxis = 2;
    long naxes[ 2 ] { static_cast< long >( output.ncolumns() ), static_cast< long >( output.nrows() ) };
    creator.create< T, U >( output.block(), frame(), naxis, naxes );
    creator.create< T, U >( invvar.block(), frame() + "_INVVAR", naxis, naxes );
    creator.create        (  flags.block(), frame() + "_FLAGS" , naxis, naxes );

    logger().debug( "Saving frame tuple", frame(), "to", path, "[DONE]" );
    logger().debug( "Output 3 x", output.block().size(), "pixels, each", output.ncolumns(), "columns x", output.nrows(), "rows" );

}

// Validate command line.  Exception if looks wrong.

template< class InstrumentTraits >
inline void C3::Serial< InstrumentTraits >::_validate_command_line( int& argc, char**& argv )
{
    if( argc < 3 ) throw C3::Exception( "Bad command line. Need executable and at least two arguments." );
}

// Parse config.  Exception if it can't be read or parsed.

template< class InstrumentTraits >
inline void C3::Serial< InstrumentTraits >::_init_config( int& argc, char**& argv )
{
    std::ifstream stream( argv[ 1 ] );
    if( ! stream ) throw C3::Exception::create( "Can't open config:", argv[ 1 ] );
    _config = YAML::Load( stream );
}

// Validate frame identifier given in config.  In serial mode, frame identifier
// is required in the config.  Exception if not found there, or else is bad.

template< class InstrumentTraits >
inline void C3::Serial< InstrumentTraits >::_validate_frame()
{
    if( ! _config[ "frame" ] ) throw C3::Exception( "No frame identifier in config. Specify one." );
    auto frame = _config[ "frame" ].template as< std::string >();
    if( InstrumentTraits::frame_exists( frame ) ) return;
    throw C3::Exception::create( "Bad frame identifier in config:", frame, "... Consult instrument definition." );
}

// Initiate logger.

template< class InstrumentTraits >
inline void C3::Serial< InstrumentTraits >::_init_logger()
{
    _config[ "logger" ] ? _init_logger_defined() : _init_logger_default();
    logger().debug( "Logger initiated for frame:", frame() );
}

// Initate logger based on config contents.

template< class InstrumentTraits >
inline void C3::Serial< InstrumentTraits >::_init_logger_defined()
{

    // Shortcut to node.

    const YAML::Node& node = _config[ "logger" ];

    // Minimum log message level.  Default from Loglevel enum class definition.

    C3::LogLevel level = C3::LogLevel::DEFAULT;
    if( node[ "loglevel" ] ) level = C3::loglevel_enum( node[ "loglevel" ].template as< std::string >() );

    // Initiate a standard logger (log to standard output) if there no logger
    // filename prefix in config.

    if( ! node[ "prefix" ] )
    {
        _logger.reset( new C3::StandardLogger( level ) );
        return;
    }

    // Otherwise initiate file logger.  First compute log-file path.  Default
    // is current working directory (dot).  Trailing slash added if necessary.

    std::string path( "." );
    if( node[ "path" ] ) path = node[ "path" ].template as< std::string >();
    if( path.back() != '/' ) path += "/";

    // Concatenate path, prefix, exposure lane (always zero in serial),
    // frame identifier, and log-file suffix to create file logger path.

    std::string fullpath = path + node[ "prefix" ].template as< std::string >() + ".000." + frame() + ".log";
    _logger.reset( new C3::FileLogger( fullpath, level ) );

}

// Initiate default logger (standard one).

template< class InstrumentTraits >
inline void C3::Serial< InstrumentTraits >::_init_logger_default()
{
    _logger.reset( new C3::StandardLogger() );
}

// Initiate task queue.

template< class InstrumentTraits >
inline void C3::Serial< InstrumentTraits >::_init_task_queue( int& argc, char**& argv )
{
    for( auto i = 2; i < argc; ++ i ) _task_files.push( argv[ i ] );
    logger().debug( "Task files in queue:", _task_files.size() );
}

