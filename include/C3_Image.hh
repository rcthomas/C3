#ifndef C3__IMAGE
#define C3__IMAGE

namespace C3
{

    template< typename T > class Column;
    template< typename T > class Row;
    template< typename T > class View;

    /// @class Image
    /// @brief Rectangular collection of pixels.

    template< typename T >
    class Image
    {

        public :

            /// Pixel type.
            typedef T value_type;

            /// Create uninitialized image.
            static Image create( const size_t ncols, const size_t nrows );

            /// Create image and initialize with a scalar value.
            static Image create( const size_t ncols, const size_t nrows, const T scalar );

            /// Constructor, copy.
            Image( const Image& image );

            /// Destructor.
            ~Image();

            /// Size, shape.
            ///@{
            size_t ncols() const { return _ncols; }
            size_t nrows() const { return _nrows; }
            size_t size()  const { return _ncols * _nrows; }
            ///@}

            /// Pixel access.
            ///@{
            T  operator () ( const size_t j, const size_t k ) const { return _data[ j + _ncols * k ]; }
            T& operator () ( const size_t j, const size_t k )       { return _data[ j + _ncols * k ]; }
            ///@}

            /// Data access.
            ///@{
            T*       data()       { return _data; }
            const T* data() const { return _data; }
            ///@}

            /// Operators, image argument.
            ///@{
            Image& operator =  ( const Image& image );
            Image& operator += ( const Image& image );
            Image& operator -= ( const Image& image );
            Image& operator *= ( const Image& image );
            Image& operator /= ( const Image& image );
            bool   operator == ( const Image& image ) const;
            bool   operator != ( const Image& image ) const;
            ///@}

            /// Operators, column argument.
            ///@{
            Image& operator =  ( const Column< T >& column );
            Image& operator += ( const Column< T >& column );
            Image& operator -= ( const Column< T >& column );
            Image& operator *= ( const Column< T >& column );
            Image& operator /= ( const Column< T >& column );
            bool   operator == ( const Column< T >& column ) const;
            bool   operator != ( const Column< T >& column ) const;
            ///@}

            /// Operators, row argument.
            ///@{
            Image& operator =  ( const Row< T >& row );
            Image& operator += ( const Row< T >& row );
            Image& operator -= ( const Row< T >& row );
            Image& operator *= ( const Row< T >& row );
            Image& operator /= ( const Row< T >& row );
            bool   operator == ( const Row< T >& row ) const;
            bool   operator != ( const Row< T >& row ) const;
            ///@}

            /// Operators, view argument.
            ///@{
            Image& operator =  ( const View< T >& view );
            Image& operator += ( const View< T >& view );
            Image& operator -= ( const View< T >& view );
            Image& operator *= ( const View< T >& view );
            Image& operator /= ( const View< T >& view );
            bool   operator == ( const View< T >& view ) const;
            bool   operator != ( const View< T >& view ) const;
            ///@}

            /// Operators, scalar argument.
            ///@{
            Image& operator =  ( const T scalar );
            Image& operator += ( const T scalar );
            Image& operator -= ( const T scalar );
            Image& operator *= ( const T scalar );
            Image& operator /= ( const T scalar );
            bool   operator == ( const T scalar ) const;
            bool   operator != ( const T scalar ) const;
            ///@}

            /// Cast to image of another type.
            template< typename U > operator Image< U > () const;

        private :

            /// Constructor.
            Image( const size_t ncols, const size_t nrows, T* data );

            size_t _ncols; ///< Number of columns.
            size_t _nrows; ///< Number of rows.
            T*     _data;  ///< Raw pixels.

    };

    /// Stream output.
    template< typename T > std::ostream& operator << ( std::ostream& stream, const Image< T >& image );

}

#include "C3_Image.inl.hh"

#endif
