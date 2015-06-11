#ifndef C3_ASSIGN_HH
#define C3_ASSIGN_HH

#include "C3_Congruent.hh"
#include "detail/C3_Assignment.hh"

namespace C3
{

    /// Assign pixel or container source to a container destination.
    template< class Destination, class Source >
    inline Destination& assign( Destination& dest, const Source& src )
    {
        assert( congruent( dest, src ) );
        return detail::assign( dest, src );
    }

}

#endif
