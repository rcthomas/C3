#ifndef C3_COLUMN_HH
#define C3_COLUMN_HH

#include "C3_OwnedBlock.hh"

namespace C3
{

    /// @class Column
    /// @brief Vertical arrangement of pixels.

    template< class T >
    class Column : public OwnedBlock< T >
    {

        public :    // Public type definitions.

            using super_type = OwnedBlock< T >;                 ///< Super-class type.
            using value_type = typename super_type::value_type; ///< Content type.

        public :    // Public methods.

            /// Constructor.
            explicit Column( const size_type nrows ) noexcept : super_type( nrows ) {}

            /// Number of rows.
            size_type nrows() const { return this->size(); }

            /// Coordinate access.
            ///@{
            value_type& operator() ( const size_type k )       { return (*this)[ k ]; }
            value_type  operator() ( const size_type k ) const { return (*this)[ k ]; }
            ///@}

    };

}

#endif
