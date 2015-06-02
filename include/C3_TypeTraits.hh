#ifndef C3_TYPE_TRAITS_HH
#define C3_TYPE_TRAITS_HH

namespace C3
{

    /// Checks if type Type is a Block.
    template< class Type > struct IsBlock;

    /// Checks if type Source is arithmetic and convertible to type T.
    template< class T, class Source > struct IsCompatibleScalar;

    /// Checks if type Source is assignable to block type Destination.
    template< class Destination, class Source > struct IsAssignable;

}

#include "C3_TypeTraits.inl.hh"

#endif
