#ifndef C3_STACK_HH
#define C3_STACK_HH

#include "C3_OwnedBlock.hh"

namespace C3
{

    /// @class Stack
    /// @brief Three-dimensional arrangement of pixels.
    ///
    /// This is a frame stack.  First coordinate indexes over frame, second
    /// indexes over column, third indexes over row.  This means that pixels at
    /// a given column and row in the stack are contiguously arranged in memory.

    template< class T >
    class Stack 
    {

        public :    // Public methods.

            /// Constructor.
            Stack( const size_type nframes, const size_type ncolumns, const size_type nrows ) noexcept : 
                _block( nframes * ncolumns * nrows ), _nframes( nframes ), _ncolumns( ncolumns ), _nrows( nrows ) {}

            /// Underlying block.
            ///@{
                  OwnedBlock< T >& block()       { return _block; }
            const OwnedBlock< T >& block() const { return _block; }
            ///@}

            /// Number of columns and rows.
            ///@{
            size_type nframes()  const { return _nframes;  }
            size_type ncolumns() const { return _ncolumns; }
            size_type nrows()    const { return _nrows;    }
            ///@}

            /// Coordinate access.
            ///@{
            T& operator() ( const size_type i, const size_type j, const size_type k )       { return _block[ i + nframes() * ( j + ncolumns() * k ) ]; }
            T  operator() ( const size_type i, const size_type j, const size_type k ) const { return _block[ i + nframes() * ( j + ncolumns() * k ) ]; }
            ///@}

        private :   // Private data members.

            OwnedBlock< T > _block;     ///< Content.
            size_type       _nframes;   ///< Total frames.
            size_type       _ncolumns;  ///< Total columns.
            size_type       _nrows;     ///< Total rows.

    };

}

#endif
