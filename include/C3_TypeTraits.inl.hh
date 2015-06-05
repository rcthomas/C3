
// Container traits primary template.

template< class T >
struct C3::ContainerTraits
{
    using value_type = T;
    static bool const is_container = false;
};

// OwnedBlock specialization of container traits template.

template< class T >
struct C3::ContainerTraits< C3::OwnedBlock< T > >
{
    using value_type = T;
    static bool const is_container = true;
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

template< class T >
struct C3::PixelTraits
{
    static bool const is_pixel = std::is_arithmetic< T >::value;
};

//

template< class Destination, class Source >
struct C3::OperationTraits
{

    static bool const src_is_pixel = C3::ContainerTraits< Destination >::is_container
        && C3::PixelTraits< Source >::is_pixel
        && std::is_convertible< Source, typename C3::ContainerTraits< Destination >::value_type >::value;

    static bool const dest_is_pixel = C3::ContainerTraits< Source >::is_container
        && C3::PixelTraits< Destination >::is_pixel
        && std::is_convertible< Destination, typename C3::ContainerTraits< Source >::value_type >::value;
    
    using congruence_type = typename std::conditional< src_is_pixel || dest_is_pixel, PixelCongruence, NullType >::type;
    using assignment_type = typename std::conditional< src_is_pixel || dest_is_pixel,  FillAssignment, NullType >::type;

};

//

template< class T, class U >
struct C3::OperationTraits< C3::OwnedBlock< T >, C3::OwnedBlock< U > >
{
    using congruence_type = SizeCongruence;
    using assignment_type = CopyAssignment;
};

//

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
    using assignment_type = ColumnFrameAssignment;
};

template< class T, class U >
struct C3::OperationTraits< C3::Stack< T >, C3::Column< U > >
{
    using congruence_type = RowCongruence;
    using assignment_type = ColumnStackAssignment;
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
