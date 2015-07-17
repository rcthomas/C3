#ifndef C3_ASSIGN_HH
#define C3_ASSIGN_HH

#include "C3_Congruent.hh"

namespace C3
{

    /// Apply binary operator and assign result to destination.
    template< template< class > class Operator, class Destination, class Source > 
    Destination& assign( Destination& dest, const Source& src );

    /// Assign pixel or container source to a container destination.
    template< class Destination, class Source > 
    Destination& assign( Destination& dest, const Source& src );

}

#include "inline/C3_Assign.hh"

#endif
