#ifndef C3_BLOCK_HH
#define C3_BLOCK_HH

#include "C3.hh"

namespace C3
{

    /// @class Block
    /// @brief ...
    ///
    /// Long spiel here...

    template< class T >
    class Block
    {

        public :    // Public type definitions.

            using value_type = T; ///< Content type.

        public :    // Public methods.

            /// Array subscript access.
            ///@{
            value_type& operator [] ( const size_type pos )       { return _data[ pos ]; }
            value_type  operator [] ( const size_type pos ) const { return _data[ pos ]; }
            ///@}

            /// Total elements.
            size_type size() const { return _size; }

            /// Native C++ array access.
            ///@{
                  value_type* data()       { return _data; }
            const value_type* data() const { return _data; }
            ///@}

            /// Start of native C++ array.
            ///@{
                  value_type* begin()       { return data(); }
            const value_type* begin() const { return data(); }
            ///@}
           
            /// End of native C++ array.
            ///@{
                  value_type* end()       { return data() + size(); }
            const value_type* end() const { return data() + size(); }
            ///@}

        protected : // Protected methods.

            /// Constructor.
            Block( const size_type size, value_type* data ) noexcept :
                _size( size ), _data( data ) {}

            /// Destructor.
            ~Block() = default;

        protected : // Protected data members.

            size_type   _size;  ///< Total elements.
            value_type* _data;  ///< Content.

    };

}

#endif
