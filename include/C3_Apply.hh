#ifndef C3_APPLY_HH
#define C3_APPLY_HH

namespace C3
{

    /// Apply binary operator to block with scalar argument.
    template< template< class > class Operator, class Block >
    Block& apply( Block& dest, const typename Block::value_type src );

    /// Apply binary operator to block with block argument of the same size.
    template< template< class > class Operator, class Block >
    Block& apply( Block& dest, const Block& src );

    /// Apply binary operator to block with repeated block argument.
    template< template< class > class Operator, class Block >
    Block& apply( Block& dest, const Block& src, const typename Block::size_type count );

}

#include "C3_Apply.inl.hh"

#endif
