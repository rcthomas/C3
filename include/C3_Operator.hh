#ifndef C3__OPERATOR
#define C3__OPERATOR

namespace C3
{

    // T should probably be floating point type but for min/max doesn't matter.

    /// @class Min
    /// @brief Reduce operation policy: computes the minimum value of an array.

    template< typename T >
    struct Min
    {
        static T compute( const size_t size, T* array );
    };

    /// @class Max
    /// @brief Reduce operation policy: computes the maximum value of an array.

    template< typename T >
    struct Max
    {
        static T compute( const size_t size, T* array );
    };

    /// @class Mean
    /// @brief Reduce operation policy: computes the mean value of an array.

    template< typename T >
    struct Mean
    {
        static T compute( const size_t size, T* array );
    };

    /// @class Stdev
    /// @brief Reduce operation policy: computes the standard deviation of an array.

    template< typename T >
    struct Stdev
    {
        static T compute( const size_t size, T* array );
    };

    /// @class Median
    /// @brief Reduce operation policy: computes median of an array.

    template< typename T >
    struct Median
    {
        static T compute( const size_t size, T* array );
    };

    /// @class Mad
    /// @brief Reduce operation policy: computes median absolute deviation of an array.

    template< typename T >
    struct Mad
    {
        static T compute( const size_t size, T* array );
    };

}

#include "C3_Operator.inl.hh"

#endif
