#ifndef C3_CONGRUENT_HH
#define C3_CONGRUENT_HH

#include "detail/C3_Congruence.hh"

namespace C3
{

    /// Check if two pixel containers have allowed congruence.
    template< class T, class U > 
    inline bool congruent( const T& lhs, const U& rhs )
    {
        return congruent( lhs, rhs, typename CongruenceTraits< T, U >::type );    
    }

}

#endif
