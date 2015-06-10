#ifndef C3_COLUMN_HH
#define C3_COLUMN_HH

#include "C3_OwnedBlock.hh"

namespace C3
{

    /// @class Column
    /// @brief Vertical arrangement of pixels.

    template< class T >
    class Column
    {

        public :    // Public methods.

            /// Constructor.
            explicit Column( const size_type nrows ) noexcept : 
                _block( nrows ) {}

            /// Underlying block.
            ///@{
                  OwnedBlock< T >& block()       { return _block; }
            const OwnedBlock< T >& block() const { return _block; }
            ///@}

            /// Number of rows.
            size_type nrows() const { return _block.size(); }

            /// Coordinate access.
            ///@{
            T& operator() ( const size_type k )       { return _block[ k ]; }
            T  operator() ( const size_type k ) const { return _block[ k ]; }
            ///@}

        private :   // Private data members.

            OwnedBlock< T > _block; ///< Content.

    };

}

#endif
