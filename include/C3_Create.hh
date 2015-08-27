#ifndef C3_CREATE_HH
#define C3_CREATE_HH

/// @file

namespace C3
{

    /// From prototype instance with same value type.

    template< class Container >
    Container create( const Container& src );

    /// From prototype instance with possibly different value type.

    template< class T, template< class > class Container, class U >
    Container< T > create( const Container< U >& src );

    // New containers resulting from unary operators or binary operators may be implemented here too.
    // Note in docs that creating from a view creates a frame not a view.

}

#include "inline/C3_Create.hh"

#endif
