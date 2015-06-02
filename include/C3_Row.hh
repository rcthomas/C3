#ifndef C3_ROW_HH
#define C3_ROW_HH

#include "C3_OwnedBlock.hh"

namespace C3
{

    /// @class
    /// @brief Horizontal arrangement of pixels.

    template< class T >
    class Row : public OwnedBlock< T >
    {

        public :    // Public type definitions.

            using super_type = OwnedBlock< T >;                 ///< Super-class type.
            using value_type = typename super_type::value_type; ///< Content type.

        public :    // Public methods.

            /// Constructor.
            explicit Row( const size_type ncolumns ) noexcept : super_type( ncolumns ) {}

            /// Number of columns.
            size_type ncolumns() const { return this->size(); }

            /// Coordinate access.
            ///@{
            value_type& operator() ( const size_type j )       { return (*this)[ j ]; }
            value_type  operator() ( const size_type j ) const { return (*this)[ j ]; }
            ///@}

    };

}

#endif
