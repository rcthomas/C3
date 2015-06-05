#ifndef C3_TYPE_TRAITS_HH
#define C3_TYPE_TRAITS_HH

namespace C3
{
    template< class T > class OwnedBlock;
    template< class T > class Column;
    template< class T > class Row;
    template< class T > class Frame;
    template< class T > class Stack;
}

namespace C3
{

    /// Container traits type.
    template< class T > struct ContainerTraits;

    /// Pixel traits type.
    template< class T > struct PixelTraits;

    /// Operation type traits.
    template< class Destination, class Source > struct OperationTraits;

    /// Null type.
    struct NullType {};

    /// Congruence types.
    struct PixelCongruence  {};
    struct SizeCongruence   {};
    struct ColumnCongruence {};
    struct RowCongruence    {};
    struct FrameCongruence  {};
    struct StackCongruence  {};

    /// Assignment types.
    struct FillAssignment           {};
    struct CopyAssignment           {};
    struct ColumnFrameAssignment    {};
    struct ColumnStackAssignment    {};
    struct RowFrameAssignment       {};
    struct RowStackAssignment       {};
    struct FrameStackAssignment     {};

}

#include "C3_TypeTraits.inl.hh"

#endif
