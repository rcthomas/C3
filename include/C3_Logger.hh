#ifndef C3_LOGGER_HH
#define C3_LOGGER_HH

namespace C3
{

    /// @class LogLevel
    /// @brief Standard message severity levels.

    enum class LogLevel : unsigned int
    {
        UNDEFINED, DEBUG, INFO, WARNING, ERROR, FATAL, DEFAULT = WARNING
    };

    /// Log-level enum to string.
    std::string loglevel_string( const LogLevel level, const bool padded = false );

    /// Log-level string to enum.
    LogLevel loglevel_enum( std::string string );

    /// @class Logger
    /// @brief Simple logger.

    class Logger
    {

        public :    // Public methods.

            /// No copy constructor.
            Logger( const Logger& logger ) = delete;

            /// No copy assignment operator.
            Logger& operator = ( const Logger& logger ) = delete;

            /// Virtual destructor.
            virtual ~Logger() = default;

            /// Logger report level.
            ///@{
            LogLevel& level()       { return _level; }
            LogLevel  level() const { return _level; }
            ///@}

            /// Message loggers.
            ///@{
            template< class... T > void debug  ( const T&... args ) { emit( LogLevel::DEBUG  , args... ); }
            template< class... T > void info   ( const T&... args ) { emit( LogLevel::INFO   , args... ); }
            template< class... T > void warning( const T&... args ) { emit( LogLevel::WARNING, args... ); }
            template< class... T > void error  ( const T&... args ) { emit( LogLevel::ERROR  , args... ); }
            template< class... T > void fatal  ( const T&... args ) { emit( LogLevel::FATAL  , args... ); }
            ///@}

            /// Message logger for any level.
            template< class... T > void emit( const LogLevel level, const T&... args );
        
        protected : // Protected methods.
            
            /// Constructor for derived classes.
            explicit Logger( const LogLevel level ) :
                _level( level ) {}

        private :   // Private methods.
            
            /// True if a message level is ignored.
            bool _ignored( const LogLevel level ) const { return level < _level; }
            
            /// Formatted timestamp. 
            std::string _timestamp() const;
            
            /// Formatted level string.
            std::string _level_text( const LogLevel level ) const;

            /// Stream a single formatted argument.
            template< class T > void _format( const T& arg );

            /// Recursively stream formatted arguments.
            template< class U, class... T > void _format( const U& head, const T&... tail );

            /// Access to derived class stream.
            virtual std::ostream& _stream() = 0;

        private :   // Private data members.

            LogLevel    _level;     ///< Minimum log level to report messages for.

    };

}

#include "inline/C3_Logger.hh"

#endif
