#ifndef C3__REDUCE
#define C3__REDUCE

namespace C3
{

    template< typename T > class Column;
    template< typename T > class Row;
    template< typename T > class Image;
    template< typename T > class Stack;
    template< typename T > class View;

    /// @class Reduce
    /// @brief Reduction template: primary version.

    template< template< typename > class O, typename T, typename U >
    struct Reduce
    {
        static U compute( const T& input );
    };

    /// @class Reduce
    /// @brief Reduction template: overload for View input, otherwise the primary template.

    template< template< typename > class O, typename T, typename U >
    struct Reduce< O, View< T >, U >
    {
        static U compute( const View< T >& input );
    };

    /// @class Reduce
    /// @brief Reduction template: overload for Image input, Column output.

    template< template< typename > class O, typename T, typename U >
    struct Reduce< O, Image< T >, Column< U > >
    {
        static Column< U > compute( const Image< T >& input );
    };

    /// @class Reduce
    /// @brief Reduction template: overload for View input, Column output.

    template< template< typename > class O, typename T, typename U >
    struct Reduce< O, View< T >, Column< U > >
    {
        static Column< U > compute( const View< T >& input );
    };

    /// @class Reduce
    /// @brief Reduction template: overload for Image input, Row output.

    template< template< typename > class O, typename T, typename U >
    struct Reduce< O, Image< T >, Row< U > >
    {
        static Row< U > compute( const Image< T >& input );
    };

    /// @class Reduce
    /// @brief Reduction template: overload for View input, Row output.

    template< template< typename > class O, typename T, typename U >
    struct Reduce< O, View< T >, Row< U > >
    {
        static Row< U > compute( const View< T >& input );
    };

    /// @class Reduce
    /// @brief Reduction template: overload for Stack input, Image output.

    template< template< typename > class O, typename T, typename U >
    struct Reduce< O, Stack< T >, Image< U > >
    {
        static Image< U > compute( const Stack< T >& stack );
    };

}

#include "C3_Reduce.inl.hh"

#endif
