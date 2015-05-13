#ifndef C3__BLOCK
#define C3__BLOCK

#include <cstddef>

namespace C3
{

    /// @class Block
    /// @brief Native C++ type pixel array block.

    template< typename T >
    class Block
    {

        public :

            /// Index and size type.
            typedef size_t size_type;
            
            /// Pixel type.
            typedef T value_type;

        public :

            /// Constructor for uninitialized block.
            explicit Block( const size_type size ) noexcept;

            /// Deep copy constructor.
            Block( const Block& block ) noexcept;

            /// Move constructor.
            Block( Block&& block ) noexcept;

            /// Deep copy assignment operator.
            Block& operator = ( const Block& block ) noexcept;

            /// Move assignment operator.
            Block& operator = ( Block&& block ) noexcept;

            /// Destructor.
            ~Block();

            /// Number of elements.
            size_type size() const { return _size; }

            /// Element access.
            ///@{
            T  operator [] ( const size_type pos ) const { return _data[ pos ]; }
            T& operator [] ( const size_type pos )       { return _data[ pos ]; }
            ///@}

            /// Data access from offset.
            ///@{
            const T* data( const size_type offset = 0 ) const { return _data; }
            T*       data( const size_type offset = 0 )       { return _data; }
            ///@}

        private :

            size_type   _size;  ///< Number of elements.
            T*          _data;  ///< Pixel values.

    };

}

#include "C3_Block.inl.hh"

#endif
