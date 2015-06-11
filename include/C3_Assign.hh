#ifndef C3_ASSIGN_HH
#define C3_ASSIGN_HH

#include "C3_Congruent.hh"
#include "detail/C3_Assignment.hh"

namespace C3
{

    /// Apply binary operator and assign result to destination.
    template< template< class > class Operator, class Destination, class Source >
    inline Destination& assign( Destination& dest, const Source& src )
    {
        assert( congruent( dest, src ) );
        return detail::assign< Operator >( dest, src );
    }

    /// Assign pixel or container source to a container destination.
    template< class Destination, class Source >
    inline Destination& assign( Destination& dest, const Source& src )
    {
        return assign< detail::Identity >( dest, src );
    }

}

#endif
