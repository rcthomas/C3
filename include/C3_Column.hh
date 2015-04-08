#ifndef C3__COLUMN
#define C3__COLUMN

namespace C3
{

    /// @class Column
    /// @brief Vertical semantic arrangement of pixels.

    template< typename T >
    class Column
    {

        public :

            /// Pixel type.
            typedef T value_type;

            /// Create uninitialized column.
            static Column create( const size_t size );

            /// Create column and initialize with a scalar value.
            static Column create( const size_t size, const T scalar );

            /// Constructor, copy.
            Column( const Column& column );

            /// Destructor.
            ~Column();

            /// Size and shape.
            ///@{
            size_t ncols() const { return     1; }
            size_t nrows() const { return _size; }
            size_t size()  const { return _size; }
            ///@}

            /// Pixel access.
            ///@{
            T  operator () ( const size_t k ) const { return _data[ k ]; }
            T& operator () ( const size_t k )       { return _data[ k ]; }
            ///@}

            /// Data access.
            ///@{
            T*       data()       { return _data; }
            const T* data() const { return _data; }
            ///@}

            /// Operators, column argument.
            ///@{
            Column& operator =  ( const Column& column );
            Column& operator += ( const Column& column );
            Column& operator -= ( const Column& column );
            Column& operator *= ( const Column& column );
            Column& operator /= ( const Column& column );
            bool    operator == ( const Column& column ) const;
            bool    operator != ( const Column& column ) const;
            ///@}

            /// Operators, scalar argument.
            ///@{
            Column& operator =  ( const T scalar );
            Column& operator += ( const T scalar );
            Column& operator -= ( const T scalar );
            Column& operator *= ( const T scalar );
            Column& operator /= ( const T scalar );
            bool    operator == ( const T scalar ) const;
            bool    operator != ( const T scalar ) const;
            ///@}

            /// Cast to column of another type.
            template< typename U > operator Column< U > () const;

        private :

            /// Constructor.
            Column( const size_t size, T* data );

            size_t  _size;  ///< Number of pixels.
            T*      _data;  ///< Raw pixels.

    };

    /// Stream output.
    template< typename T > std::ostream& operator << ( std::ostream& stream, const Column< T >& column );

}

#include "C3_Column.inl.hh"

#endif
