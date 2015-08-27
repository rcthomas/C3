#ifndef C3_STACK_HH
#define C3_STACK_HH

#include "C3_Block.hh"

namespace C3
{

    /// @class Stack
    /// @brief Three-dimensional arrangement of pixels.
    ///
    /// This is a frame stack.  First coordinate indexes over frame, second
    /// indexes over column, third indexes over row.  This means that pixels at
    /// a given column and row in the stack are contiguously arranged in memory.

    template< class T >
    class Stack : public Block< T >
    {

        public :    // Public methods.

            /// Constructor.
            Stack( const size_type nframes, const size_type ncolumns, const size_type nrows ) noexcept;

            /// Initializing constructor.
            Stack( const size_type nframes, const size_type ncolumns, const size_type nrows, const T pixel ) noexcept;

            /// Number of columns and rows.
            ///@{
            size_type nframes()  const { return _nframes;  }
            size_type ncolumns() const { return _ncolumns; }
            size_type nrows()    const { return _nrows;    }
            ///@}

            /// Coordinate access.
            /// @{
            T& operator() ( const size_type i, const size_type j, const size_type k )
                { return (*this)[ i + nframes() * ( j + ncolumns() * k ) ]; }
            T  operator() ( const size_type i, const size_type j, const size_type k ) const 
                { return (*this)[ i + nframes() * ( j + ncolumns() * k ) ]; }
            /// @}

            /// Pixel assignment.
            Stack& operator = ( const T pixel ) noexcept;

            /// Value type conversion.
            template< class U >
            operator Stack< U >() const noexcept;

        private :   // Private data members.

            size_type   _nframes;   ///< Total frames.
            size_type   _ncolumns;  ///< Total columns.
            size_type   _nrows;     ///< Total rows.

    };

}

#include "inline/C3_Stack.hh"

#endif
