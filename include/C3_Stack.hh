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
    class Stack : public OwnedBlock< T >
    {

        public :    // Public type definitions.

            using super_type = OwnedBlock< T >;                 ///< Super-class type.
            using value_type = typename super_type::value_type; ///< Content type.

        public :    // Public methods.

            /// Constructor.
            explicit Stack( const size_type nframes, const size_type ncolumns, const size_type nrows ) noexcept : 
                super_type( nframes * ncolumns * nrows ), _nframes( nframes ), _ncolumns( ncolumns ), _nrows( nrows ) {}

            /// Total frames, columns, rows.
            ///@{
            size_type nframes()  const { return _nframes;  }
            size_type ncolumns() const { return _ncolumns; }
            size_type nrows()    const { return _nrows;    }
            ///@}

            /// Coordinate access.
            ///@{
            value_type& operator() ( const size_type i, const size_type j, const size_type k )       { return (*this)[ i + _nframes * ( j + _ncolumns * k ) ]; }
            value_type  operator() ( const size_type i, const size_type j, const size_type k ) const { return (*this)[ i + _nframes * ( j + _ncolumns * k ) ]; }
            ///@}

        private :   // Private data members.

            size_type _nframes;     ///< Total frames.
            size_type _ncolumns;    ///< Total columns.
            size_type _nrows;       ///< Total rows.

    };

}

#endif
