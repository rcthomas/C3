
// Named constructor for list of arguments to format into message string.

template< class... T >
inline C3::Exception C3::Exception::create( const T&... args )
{
    return C3::Exception( format( args... ) );
}

// Format list of arguments into message string.

template< class... T >
inline std::string C3::Exception::format( const T&... args )
{
    std::stringstream ss;
    ss << std::boolalpha;
    return _format( ss, args... ).str();
}

// Format argument of any type into string stream.

template< class T >
inline std::stringstream& C3::Exception::_format( std::stringstream& stream, const T& arg )
{
    stream << arg;
    return stream;
}

// Recursively format arguments of any type into string stream.

template< class T, class... U >
inline std::stringstream& C3::Exception::_format( std::stringstream& stream, const T& head, const U&... tail )
{
    stream << head << " ";
    return _format( stream, tail... );
}
