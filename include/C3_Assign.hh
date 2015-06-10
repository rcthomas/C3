#ifndef C3_ASSIGN_HH
#define C3_ASSIGN_HH

namespace C3
{

    /// Assign pixel or container source to a container destination.
    template< class Destination, class Source >
    inline Destination& assign( Destination& dest, const Source& src );

}

#include "detail/C3_Assignment.hh"

#endif
