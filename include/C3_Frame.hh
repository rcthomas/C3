#ifndef C3_FRAME_HH
#define C3_FRAME_HH

#include "C3_OwnedBlock.hh"

namespace C3
{

    /// @class Frame
    /// @brief Two-dimensional arrangement of pixels.

    template< class T >
    class Frame
    {

        public :    // Public methods.

            /// Constructor.
            Frame( const size_type ncolumns, const size_type nrows ) noexcept : 
                _block( ncolumns * nrows ), _ncolumns( ncolumns ), _nrows( nrows ) {}

            /// Underlying block.
            ///@{
                  OwnedBlock< T >& block()       { return _block; }
            const OwnedBlock< T >& block() const { return _block; }
            ///@}

            /// Number of columns and rows.
            ///@{
            size_type ncolumns() const { return _ncolumns; }
            size_type nrows()    const { return _nrows;    }
            ///@}

            /// Coordinate access.
            ///@{
            T& operator() ( const size_type j, const size_type k )       { return _block[ j + _ncolumns * k ]; }
            T  operator() ( const size_type j, const size_type k ) const { return _block[ j + _ncolumns * k ]; }
            ///@}

        private :   // Private data members.

            OwnedBlock< T > _block;     ///< Content.
            size_type       _ncolumns;  ///< Total columns.
            size_type       _nrows;     ///< Total rows.

    };

}

#endif
