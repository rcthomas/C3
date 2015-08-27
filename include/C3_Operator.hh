#ifndef C3_OPERATOR_HH
#define C3_OPERATOR_HH

namespace C3
{

    /// Arithmetic assignment operators.
    /// @{

    template< class Destination, class Source > 
    Destination& operator += ( Destination& dest, const Source& src );

    template< class Destination, class Source > 
    Destination& operator -= ( Destination& dest, const Source& src );

    template< class Destination, class Source > 
    Destination& operator *= ( Destination& dest, const Source& src );

    template< class Destination, class Source > 
    Destination& operator /= ( Destination& dest, const Source& src );

    /// @}

    /// Arithmetic creation operators
    /// @{

//  /// Unary minus.
//  template< class T > T operator - ( const T& src );

//  /// Addition.
//  template< class T, class U > T operator + ( const T& lsrc, const U& rsrc );

//  /// Subtraction.
//  template< class T, class U > T operator - ( const T& lsrc, const U& rsrc );

//  /// Multiplication.
//  template< class T, class U > T operator * ( const T& lsrc, const U& rsrc );

//  /// Division.
//  template< class T, class U > T operator / ( const T& lsrc, const U& rsrc );

    /// @}

}

#include "inline/C3_Operator.hh"

#endif
