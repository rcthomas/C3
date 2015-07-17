#ifndef C3_CONGRUENT_HH
#define C3_CONGRUENT_HH

namespace C3
{

    /// Check if two pixel containers have allowed congruence.
    template< class T, class U > 
    bool congruent( const T& lhs, const U& rhs );

}

#include "inline/C3_Congruent.hh"

#endif
