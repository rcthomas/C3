
#include "../C3_Assign.hh"
#include "../C3_TypeTraits.hh"

// Addition assignment.

template< class Destination, class Source > 
inline Destination& C3::operator += ( Destination& dest, const Source& src ) 
{ 
    using T = typename C3::ValueType< Destination >::type;
    return C3::assign( dest, src, std::plus< T >() ); 
}

// Subtraction assignment.

template< class Destination, class Source >
inline Destination& C3::operator -= ( Destination& dest, const Source& src ) 
{ 
    using T = typename C3::ValueType< Destination >::type;
    return C3::assign( dest, src, std::minus< T >() ); 
}

// Multiplication assignment.

template< class Destination, class Source > 
inline Destination& C3::operator *= ( Destination& dest, const Source& src ) 
{ 
    using T = typename C3::ValueType< Destination >::type;
    return C3::assign( dest, src, std::multiplies< T >() ); 
}

// Division assignment.

template< class Destination, class Source > 
inline Destination& C3::operator /= ( Destination& dest, const Source& src ) 
{ 
    using T = typename C3::ValueType< Destination >::type;
    return C3::assign( dest, src, std::divides< T >() ); 
}

// // Unary minus.
// 
// template< class T > 
// inline T C3::operator - ( const T& src )
// {
//     return C3::assign( src, std::negate< typename C3::ValueType< T >::type >() );
// }
// 
// // Addition.
// 
// template< class T, class U > 
// inline T C3::operator + ( const T& lsrc, const U& rsrc )
// {
//     return C3::assign( lsrc, rsrc, std::plus< typename C3::ValueType< T >::type >() );
// }
// 
// // Subtraction.
// 
// template< class T, class U > 
// inline T C3::operator - ( const T& lsrc, const U& rsrc )
// {
//     return C3::assign( lsrc, rsrc, std::minus< typename C3::ValueType< T >::type >() );
// }
// 
// // Multiplication.
// 
// template< class T, class U > 
// inline T C3::operator * ( const T& lsrc, const U& rsrc )
// {
//     return C3::assign( lsrc, rsrc, std::multiplies< typename C3::ValueType< T >::type >() );
// }
// 
// // Division.
// 
// template< class T, class U > 
// inline T C3::operator / ( const T& lsrc, const U& rsrc )
// {
//     return C3::assign( lsrc, rsrc, std::divides< typename C3::ValueType< T >::type >() );
// }
