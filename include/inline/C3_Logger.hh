
#include <chrono>
#include <sstream>

// Log-level enum to string.

inline std::string C3::loglevel_string( const C3::LogLevel level, const bool padded )
{
    const std::string::size_type WIDTH = 10;
    std::string result;
    if( level == C3::LogLevel::DEBUG   ) result = "DEBUG";
    if( level == C3::LogLevel::INFO    ) result = "INFO";
    if( level == C3::LogLevel::WARNING ) result = "WARNING";
    if( level == C3::LogLevel::ERROR   ) result = "ERROR";
    if( level == C3::LogLevel::FATAL   ) result = "FATAL";
    if( result.empty()                 ) result = "UNDEFINED";
    if( padded ) result.resize( WIDTH, ' ' );
    return result;
}

// Log-level string to enum.

inline C3::LogLevel C3::loglevel_enum( std::string string )
{
    for( auto& c : string ) c = toupper( c );
    if( string == "DEBUG"   ) return C3::LogLevel::DEBUG;
    if( string == "INFO"    ) return C3::LogLevel::INFO;
    if( string == "WARNING" ) return C3::LogLevel::WARNING;
    if( string == "ERROR"   ) return C3::LogLevel::ERROR;
    if( string == "FATAL"   ) return C3::LogLevel::FATAL;
    return C3::LogLevel::UNDEFINED;
}

// Message logger for any level.

template< class... T >
inline void C3::Logger::emit( const C3::LogLevel level, const T&... args )
{
    if( _ignored( level ) ) return;
    _stream() << std::boolalpha;
    _stream() << _timestamp()         << " ";
    _stream() << _level_text( level ) << " ";
    _format( args... );
}

// Formatted timestamp.

inline std::string C3::Logger::_timestamp() const
{

    auto   now  = std::chrono::system_clock::now();
    auto   usec = std::chrono::duration_cast< std::chrono::microseconds >( now.time_since_epoch() );
    auto   mod  = usec.count() % 1000000;   // size_t
    time_t sec  = std::chrono::duration_cast< std::chrono::seconds >( usec ).count(); // time_t

    const std::string::size_type MAXLEN = 80;
    char buffer[ MAXLEN ];
    strftime( buffer, MAXLEN, "%Y-%m-%dT%H:%M:%S.", localtime( &sec ) );

    std::stringstream ss;
    ss << buffer;
    ss.fill( '0' );
    ss.width( 6 );
    ss << mod;

    return ss.str();

}

// Formatted level string.

inline std::string C3::Logger::_level_text( const C3::LogLevel level ) const
{
    return C3::loglevel_string( level, true );
}

// Stream a single formatted argument.

template< class T >
inline void C3::Logger::_format( const T& value )
{
    _stream() << value << std::endl;
}

// Recursively stream formatted arguments.

template< class U, class... T >
inline void C3::Logger::_format( const U& head, const T&... tail )
{
    _stream() << head << " ";
    _format( tail... );
}

