#ifndef C3_ASSIGN_HH
#define C3_ASSIGN_HH

/// @file

namespace C3
{

    /// @defgroup assignment
    /// @brief Map contents from container or pixel to another container, optionally using a binary operator first.
    ///
    /// Assignment is defined only for certain container combinations.  The following table explains which combinations
    /// are defined.  These also apply for the usual assignment operators (=, +=, -=, *=, /=) that actually just wrap
    /// a C3::assign() call.  For each source container or pixel listed on a given row, columns list destination 
    /// containers that can be assigned to.
    ///
    ///                 To
    ///                 Block   Column  Row     Frame   View    Stack
    ///     From Pixel  YES     YES     YES     YES     YES     YES
    ///          Block  YES     
    ///          Column         YES             YES     YES
    ///          Row                    YES     YES     YES
    ///          Frame                          YES     YES     YES
    ///          View                           YES     YES     YES
    ///          Stack                                          YES
    ///
    /// The first thing to note is that a pixel source can be assigned to any destination container.  The second thing
    /// to note is that a given container can always be assigned to the same kind container.  In all cases, the value 
    /// types of the container or pixel on either side of the assignment is not required to be the same.  If a binary
    /// operator is being applied in the assignment, it is up to the user to account for type dependence.  In many 
    /// cases a compiler error could result if types do not match, but this is not guaranteed.
    ///
    /// The above table describes allowed assignment combinations for containers and pixels.  Incorrect combinations
    /// based on container type will result in an error at compile time.  But in order for assignment to proceed at run
    /// time, the containers must be congruent.  For example, a Frame should only be assigned to a Stack if the two
    /// instances have the same number of rows and columns; a Column should only be assigned to a View if the two have
    /// the same number of rows.  A mismatch results in a run-time assertion failure, unless the NDEBUG macro has been
    /// defined at compile time.  
    ///
    /// @{

    /// Directly map the contents of a source container or pixel to a destination container.
    ///
    /// @par Example
    /// Zero-out a 100x2048x4096 Stack:
    ///
    ///     C3::Stack< double > stack( 100, 2048, 4096 );
    ///     C3::assign( stack, 0.0 );
    ///
    /// Of course, the same thing can be done by simply calling the Stack constructor with 0.0 as an additional
    /// argument.  Feeling cheated?  Okay another example.
    ///
    /// @par Example
    /// Initialize a 2048x4096 Frame with 4096 copies of the same 2048-columned row stacked on top of one another.
    ///
    ///     C3::Frame< double > frame( 2048, 4096 );
    ///     C3::Row< float > row( 2048 );
    ///     for( auto j = 0; j < row.ncolumns(); ++j ) row( j ) = j;
    ///     C3::assign( frame, row );
    ///
    /// You should get the feeling by now that you probably won't use this function much directly, but nevertheless here
    /// it is, part of the public interface.  More than likely you should just say "frame = row" instead of call
    /// C3::assign().
    ///
    /// @param  dest Destination container.
    /// @param  src  Source container or pixel value.
    /// @return Destination container, modified by assignment, can be ignored.

    template< class Destination, class Source >
    Destination& assign( Destination& dest, const Source& src );

    /// Apply binary operator to destination container and source container or pixel and assign result to destination.
    ///
    /// @par Example
    /// Increment every pixel in a 2048x4096 Frame by 2.0:
    ///
    ///     C3::Frame< double > frame( 2048, 4096 );
    ///     auto pos = 0;
    ///     for( auto& pixel : frame ) pixel = pos++;
    ///     C3::assign( frame, 2.0, std::plus< double >() );
    ///
    /// The same thing should be accomplished by "frame += 2.0" but other operators can be defined and used here.
    /// 
    /// @param  dest Destination container.
    /// @param  src  Source container or pixel value.
    /// @param  op   Binary operator object (future: generalize to binary function).
    /// @return Destination container, modified by assignment, can be ignored.

    template< class Destination, class Source, class BinaryOperator >
    Destination& assign( Destination& dest, const Source& src, BinaryOperator op );

}

#include "inline/C3_Assign.hh"

#endif
