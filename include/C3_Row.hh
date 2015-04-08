#ifndef C3__ROW
#define C3__ROW

namespace C3
{

    /// @class Row
    /// @brief Horizontal semantic arrangement of pixels.

    template< typename T >
    class Row
    {

        public :

            /// Pixel type.
            typedef T value_type;

            /// Create uninitialized row.
            static Row create( const size_t size );

            /// Create row and initialize with a scalar value.
            static Row create( const size_t size, const T scalar );

            /// Constructor, copy.
            Row( const Row& row );

            /// Destructor.
            ~Row();

            /// Size.
            ///@{
            size_t ncols() const { return _size; }
            size_t nrows() const { return     1; }
            size_t size()  const { return _size; }
            ///@}

            /// Pixel access.
            ///@{
            T  operator () ( const size_t j ) const { return _data[ j ]; }
            T& operator () ( const size_t j )       { return _data[ j ]; }
            ///@}

            /// Data access.
            ///@{
            T*       data()       { return _data; }
            const T* data() const { return _data; }
            ///@}

            /// Operators, row argument.
            ///@{
            Row& operator =  ( const Row& row );
            Row& operator += ( const Row& row );
            Row& operator -= ( const Row& row );
            Row& operator *= ( const Row& row );
            Row& operator /= ( const Row& row );
            bool operator == ( const Row& row ) const;
            bool operator != ( const Row& row ) const;
            ///@}

            /// Operators, scalar argument.
            ///@{
            Row& operator =  ( const T scalar );
            Row& operator += ( const T scalar );
            Row& operator -= ( const T scalar );
            Row& operator *= ( const T scalar );
            Row& operator /= ( const T scalar );
            bool operator == ( const T scalar ) const;
            bool operator != ( const T scalar ) const;
            ///@}

            /// Cast to row of another type.
            template< typename U > operator Row< U > () const;

        private :

            /// Constructor.
            Row( const size_t size, T* data );

            size_t  _size; ///< Number of pixels.
            T*      _data; ///< Raw pixels.

    };

    /// Stream output.
    template< typename T > std::ostream& operator << ( std::ostream& stream, const Row< T >& row );

}

#include "C3_Row.inl.hh"

#endif
