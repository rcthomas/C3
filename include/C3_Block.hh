#ifndef C3_BLOCK_HH
#define C3_BLOCK_HH

#include <cstdlib>

// Forward declaration.

namespace C3
{
    template< class Derived > class Block;
}

namespace C3
{

    /// @class Block
    /// @brief ...
    ///
    /// Long spiel here...

    template< class T, template< class > class Derived >
    class Block< Derived< T > >
    {

        public :    // Public type definitions.

            using size_type    = size_t;        ///< Index and size type.
            using value_type   = T;             ///< Content type.
            using derived_type = Derived< T >;  ///< Derived class type.

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

        private :   // Private methods.

            /// Cast to derived type.
            ///@{
                  derived_type* _self()       { return static_cast<       derived_type* >(this); }
            const derived_type* _self() const { return static_cast< const derived_type* >(this); }
            ///@}

        protected : // Protected data members.

            size_type   _size;  ///< Total elements.
            value_type* _data;  ///< Content.

    };

}

#endif
