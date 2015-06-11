
// Forward declarations.

namespace C3
{
    template< class T > class Column;
    template< class T > class Row;
    template< class T > class Frame;
    template< class T > class Stack;
    template< class T > class View;
}

// Pixel traits primary template.  Any arithmetic type may serve as a pixel type.

template< class T >
struct C3::PixelTraits
{
    static bool const is_pixel = std::is_arithmetic< T >::value;
};

// Container traits primary template.

template< class T >
struct C3::ContainerTraits
{
    using value_type = T;
    static bool const is_container = false;
};

// Column specialization of container traits template.

template< class T >
struct C3::ContainerTraits< C3::Column< T > >
{
    using value_type = T;
    static bool const is_container = true;
};

// Row specialization of container traits template.

template< class T >
struct C3::ContainerTraits< C3::Row< T > >
{
    using value_type = T;
    static bool const is_container = true;
};

// Frame specialization of container traits template.

template< class T >
struct C3::ContainerTraits< C3::Frame< T > >
{
    using value_type = T;
    static bool const is_container = true;
};

// Stack specialization of container traits template.

template< class T >
struct C3::ContainerTraits< C3::Stack< T > >
{
    using value_type = T;
    static bool const is_container = true;
};

//

template< class Destination, class Source >
struct C3::OperationTraits
{
    using congruence_type = NullType;
    using assignment_type = NullType;
};

// Pixel source.

template< class T, class U >
struct C3::OperationTraits< C3::Pixel< T >, C3::Pixel< T > >
{
    using congruence_type = NullType;
    using assignment_type = NullType;
};

template< class T, class U >
struct C3::OperationTraits< C3::Column< T >, C3::Pixel< T > >
{
    using congruence_type = PixelCongruence;
    using assignment_type = PixelAssignment;
};

template< class T, class U >
struct C3::OperationTraits< C3::Row< T >, C3::Pixel< T > >
{
    using congruence_type = PixelCongruence;
    using assignment_type = PixelAssignment;
};




// Column 

template< class T, class U >
struct C3::OperationTraits< C3::Column< T >, C3::Column< U > >
{
    using congruence_type = RowCongruence;
    using assignment_type = CopyAssignment;
};

template< class T, class U >
struct C3::OperationTraits< C3::Frame< T >, C3::Column< U > >
{
    using congruence_type = RowCongruence;
    using assignment_type = FillAssignment;
};

template< class T, class U >
struct C3::OperationTraits< C3::Stack< T >, C3::Column< U > >
{
    using congruence_type = RowCongruence;
    using assignment_type = FillAssignment;
};

template< class T, class U >
struct C3::OperationTraits< C3::View< T >, C3::Column< U > >
{
    using congruence_type = RowCongruence;
    using assignment_type = FillAssignment;
};

//

template< class T, class U >
struct C3::OperationTraits< C3::Row< T >, C3::Row< U > >
{
    using congruence_type = ColumnCongruence;
    using assignment_type = CopyAssignment;
};

template< class T, class U >
struct C3::OperationTraits< C3::Frame< T >, C3::Row< U > >
{
    using congruence_type = ColumnCongruence;
    using assignment_type = RowFrameAssignment;
};

template< class T, class U >
struct C3::OperationTraits< C3::Stack< T >, C3::Row< U > >
{
    using congruence_type = ColumnCongruence;
    using assignment_type = RowStackAssignment;
};

//

template< class T, class U >
struct C3::OperationTraits< C3::Column< T >, C3::Frame< U > >
{
    using congruence_type = RowCongruence;
    using assignment_type = NullType;
};

template< class T, class U >
struct C3::OperationTraits< C3::Row< T >, C3::Frame< U > >
{
    using congruence_type = ColumnCongruence;
    using assignment_type = NullType;
};

template< class T, class U >
struct C3::OperationTraits< C3::Frame< T >, C3::Frame< U > >
{
    using congruence_type = FrameCongruence;
    using assignment_type = CopyAssignment;
};

template< class T, class U >
struct C3::OperationTraits< C3::Stack< T >, C3::Frame< U > >
{
    using congruence_type = FrameCongruence;
    using assignment_type = FrameStackAssignment;
};

//

template< class T, class U >
struct C3::OperationTraits< C3::Column< T >, C3::Stack< U > >
{
    using congruence_type = RowCongruence;
    using assignment_type = NullType;
};

template< class T, class U >
struct C3::OperationTraits< C3::Row< T >, C3::Stack< U > >
{
    using congruence_type = ColumnCongruence;
    using assignment_type = NullType;
};

template< class T, class U >
struct C3::OperationTraits< C3::Frame< T >, C3::Stack< U > >
{
    using congruence_type = FrameCongruence;
    using assignment_type = NullType;
};

template< class T, class U >
struct C3::OperationTraits< C3::Stack< T >, C3::Stack< U > >
{
    using congruence_type = StackCongruence;
    using assignment_type = CopyAssignment;
};
