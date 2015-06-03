
// Forward declarations.

namespace C3
{
    template< class T > class Column;
    template< class T > class Row;
    template< class T > class Frame;
    template< class T > class Stack;
}

// Checks if a type is a Block.

template< class Type > struct C3::IsBlock                           { static const bool value = false; using value_type = Type; };
template< class T >    struct C3::IsBlock< C3::OwnedBlock< T > >    { static const bool value = true;  using value_type = T;    };
template< class T >    struct C3::IsBlock< C3::Column< T > >        { static const bool value = true;  using value_type = T;    };
template< class T >    struct C3::IsBlock< C3::Row< T >    >        { static const bool value = true;  using value_type = T;    };
template< class T >    struct C3::IsBlock< C3::Frame< T >  >        { static const bool value = true;  using value_type = T;    };
template< class T >    struct C3::IsBlock< C3::Stack< T >  >        { static const bool value = true;  using value_type = T;    };

// Checks if type Source is arithmetic and convertible to type T.

template< class T, class Source >
struct C3::IsCompatibleScalar
{
    static const bool value = std::is_arithmetic< Source >::value 
        && std::is_convertible< Source, T >::value;
};

// Checks if type Source is assignable to destination.

template< class Destination, class Source >
struct C3::IsAssignable
{
    static const bool value = C3::IsBlock< Destination >::value 
        && ( std::is_same< Destination, Source >::value || C3::IsCompatibleScalar< typename C3::IsBlock< Destination >::value_type, Source >::value );
};

// Any Block type but Stack type and convertible arithmetic scalar type can
// assign to Frame type.

template< class Source, class T >
struct C3::IsAssignable< C3::Frame< T >, Source >
{
    static const bool value = ( C3::IsBlock< Source >::value && ( ! std::is_same< C3::Stack< T >, Source >::value ) )
        || C3::IsCompatibleScalar< T, Source >::value;
};

// Any Block type and convertible arithmetic scalar type can assign to Stack
// type.

template< class Source, class T >
struct C3::IsAssignable< C3::Stack< T >, Source >
{
    static const bool value = C3::IsBlock< Source >::value
        || C3::IsCompatibleScalar< T, Source >::value;
};