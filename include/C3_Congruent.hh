#ifndef C3_CONGRUENT_HH
#define C3_CONGRUENT_HH

#include "C3_TypeTraits.hh"

namespace C3
{

    /// Returns true if the two types are congruent along one or more dimension.
    template< class T, class U > bool congruent( const T& lhs, const U& rhs );

    namespace Detail
    {
        template< class T, class U > bool congruent( const T& lhs, const U& rhs, NullType         );
        template< class T, class U > bool congruent( const T& lhs, const U& rhs, PixelCongruence  );
        template< class T, class U > bool congruent( const T& lhs, const U& rhs, SizeCongruence   );
        template< class T, class U > bool congruent( const T& lhs, const U& rhs, ColumnCongruence );
        template< class T, class U > bool congruent( const T& lhs, const U& rhs, RowCongruence    );
        template< class T, class U > bool congruent( const T& lhs, const U& rhs, FrameCongruence  );
        template< class T, class U > bool congruent( const T& lhs, const U& rhs, StackCongruence  );
    }

}

#include "C3_Congruent.inl.hh"

#endif
