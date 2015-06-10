#ifndef C3_TYPE_TRAITS_HH
#define C3_TYPE_TRAITS_HH

namespace C3
{

    /// Null type.
    struct Undefined {};

    /// Congruence types.
    struct PixelCongruence  {};
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

    template< class Head, class Tail >
    struct ContainerPairTraits
    {

        const bool head_is_pixel     = IsPixel< Head >::value;
        const bool head_is_container = IsContainer< Head >::value;

        const bool tail_is_pixel     = IsPixel< Tail >::value;
        const bool tail_is_container = IsContainer< Tail >::value;

        const bool head_pixel_tail_container = head_is_pixel     && tail_is_container;
        const bool head_container_tail_pixel = head_is_container && tail_is_pixel;

        const bool container_and_pixel = head_pixel_tail_container || head_container_tail_pixel;

    };

}

#include "C3_TypeTraits.inl.hh"

#endif
