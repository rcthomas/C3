#ifndef C3_ROW_HH
#define C3_ROW_HH

#include "C3_OwnedBlock.hh"

namespace C3
{

    /// @class Row
    /// @brief Horizontal arrangement of pixels.

    template< class T >
    class Row
    {

        public :    // Public methods.

            /// Constructor.
            explicit Row( const size_type ncolumns ) noexcept : 
                _block( ncolumns ) {}

            /// Underlying block.
            ///@{
                  OwnedBlock< T >& block()       { return _block; }
            const OwnedBlock< T >& block() const { return _block; }
            ///@}

            /// Number of columns.
            size_type ncolumns() const { return _block.size(); }

            /// Coordinate access.
            ///@{
            T& operator() ( const size_type j )       { return _block[ j ]; }
            T  operator() ( const size_type j ) const { return _block[ j ]; }
            ///@}

        private :   // Private data members.

            OwnedBlock< T > _block; ///< Content.

    };

}

#endif
