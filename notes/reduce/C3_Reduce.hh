#ifndef C3_REDUCE_HH
#define C3_REDUCE_HH

/// @file

namespace C3
{

    /// @defgroup reduction 
    /// @brief Reduce containers into lower-dimensional containers or pixels.
    ///
    /// Reduction is defined only for certain container combinations.  Here is a table listing possible combinations.
    /// For each source container listed on a given row, columns list which destination containers can be computed by
    /// reduction.
    ///
    ///                  To
    ///                  Pixel  Column  Row     Frame   View    Stack
    ///     From Column  YES    no      no      no      no      no
    ///          Row     YES    no      no      no      no      no
    ///          Frame   YES    YES     YES     no      no      no
    ///          View    YES    YES     YES     no      no      no
    ///          Stack   YES    no      no      YES     YES     no
    ///
    /// Reduction can be used to create and initialize a new container, except in the case of View.  These must always
    /// be created before the reduction, and use the in-place (three argument) reduce.
    ///
    /// @{

    /// Create destination container by reducing from source container, both having the same value type.
    ///
    /// @par Example
    /// Reduce a 2048x4096 frame and put the result in a new column containing 4096 pixels/rows having the same value
    /// type as the original frame.  This column is the sum over all columns for each row:
    ///
    ///     C3::Frame< double > frame( 2048, 4096, 2.0 );
    ///     auto column = C3::reduce< C3::Column >( frame, C3::sum() );
    ///     // ... column is a C3::Column< double > with 4096 pixels.
    ///
    /// @param  src Source container of larger dimension.
    /// @param  op Reduction operator object.
    /// @return Destination container of smaller dimension.

    template< template< class > class Destination, template< class > class Source, class T, class Operator >
    Destination< T > reduce( const Source< T >& src, Operator op );

    /// Reduce a source container into a single pixel value, both having the same value type.
    ///
    /// @par Example
    /// Sum all the pixels in a frame into a single scalar value.  The value type will be the same as the frame's value 
    /// type.
    ///
    ///     C3::Frame< double > frame( 2048, 4096, 2.0 );
    ///     auto pixel = C3::reduce( frame, C3::sum() );
    ///     // ... pixel has type double
    ///
    /// @param  src Source container.
    /// @param  op Reduction operator object.
    /// @return Destination pixel value.

    template< template< class > class Source, class T, class Operator >
    T reduce( const Source< T >& src, Operator op );

    /// Create destination container with one value type by reducing from source container with another value type.
    ///
    /// @par Example
    /// Reduce a 2048x4096 frame of value type float and put the result in a new column containing 4096 pixels/rows of 
    /// value type double (not float).  This column is the sum over all columns for each row, and the operation is done
    /// in the destination container's precision.
    ///
    ///     C3::Frame< float > frame( 2048, 4096, 2.0 );
    ///     auto row = C3::reduce< C3::Row, double >( frame, C3::sum() );
    ///     // ... column is a C3::Row< double > (not float) with 2048 pixels.
    ///
    /// @param  src Source container of larger dimension.
    /// @param  op Reduction operator object.
    /// @return Destination container of smaller dimension.

    template< template< class > class Destination, class T, template< class > class Source, class U, class Operator >
    Destination< T > reduce( const Source< U >& src, Operator op );

    /// Reduce a source container having one value type into a single pixel value having another value type.
    ///
    /// @par Example
    /// Sum all the pixels in a frame into a single scalar value of some other type.  The reduction is done at the 
    /// precision of the target pixel type.
    ///
    ///     C3::Frame< float > frame( 100, 100, 2.0 );
    ///     auto pixel = C3::reduce< double >( frame, C3::sum() );
    ///     // ... pixel has type double (not float).
    ///
    /// @param  src Source container.
    /// @param  op Reduction operator object.
    /// @return Destination pixel value.

    template< class T, template< class > class Source, class U, class Operator >
    T reduce( const Source< U >& src, Operator op );

    /// Reduce a source container to existing destination container.
    ///
    /// @par Example 1
    /// Sum all the pixels in a stack into a single frame of another type.  Here since both the destination and source
    /// container types can be inferred (they are both arguments to the reduce function) the user does not have to 
    /// supply any template parameters even if they have different value types.
    ///
    ///     C3::Stack< float > stack( 10, 100, 100, 2.0 );
    ///     C3::Frame< double > frame( 100, 100 );
    ///     frame = C3::reduce( frame, stack, C3::sum() );
    ///
    /// @par Example 2
    /// Sum all the pixels in a stack into a single pixel.  Again the argument types are known so the right template
    /// parameters are all inferred by the compiler.
    ///
    ///     C3::Stack< float > stack( 10, 100, 100, 2.0 );
    ///     double x;
    ///     C3::reduce( x, stack, C3::sum() ); // return value can be ignored
    ///
    /// @param  dest Destination container or pixel, gets modified.
    /// @param  src Source container.
    /// @param  op Reduction operator object.
    /// @return Destination container (optional).

    template< class Destination, class Source, class Operator >
    Destination& reduce( Destination& dest, const Source& src, Operator op );

    /// @}

}

#include "inline/C3_Reduce.hh"

#endif
