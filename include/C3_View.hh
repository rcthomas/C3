#ifndef C3__VIEW
#define C3__VIEW

namespace C3
{

    template< typename T > class Column;
    template< typename T > class Row;
    template< typename T > class Image;

    /// @class View
    /// @brief References a rectangular aggregation of pixels in an image.

    template< typename T >
    class View
    {

        public :

            // Pixel type.
            typedef T value_type;

            /// Create view from image and bounding box.
            static View create( Image< T >& image, const size_t stcol, const size_t ncols, const size_t strow, const size_t nrows );

            /// Constructor, copy.
            View( const View& view );

            /// Destructor.
            ~View();

            /// Size, shape.
            ///@{
            size_t ncols() const { return _ncols; }
            size_t nrows() const { return _nrows; }
            size_t size()  const { return _ncols * _nrows; }
            ///@}

            /// Pixel access.
            ///@{
            T  operator () ( const size_t j, const size_t k ) const { return _data[ j + _stride * k ]; }
            T& operator () ( const size_t j, const size_t k )       { return _data[ j + _stride * k ]; }
            ///@}

            /// Operators, view argument.
            ///@{
            View& operator =  ( const View& view );
            View& operator += ( const View& view );
            View& operator -= ( const View& view );
            View& operator *= ( const View& view );
            View& operator /= ( const View& view );
            bool  operator == ( const View& view ) const;
            bool  operator != ( const View& view ) const;
            ///@}

            /// Operators, image argument.
            ///@{
            View& operator =  ( const Image< T >& image );
            View& operator += ( const Image< T >& image );
            View& operator -= ( const Image< T >& image );
            View& operator *= ( const Image< T >& image );
            View& operator /= ( const Image< T >& image );
            bool  operator == ( const Image< T >& image ) const;
            bool  operator != ( const Image< T >& image ) const;
            ///@}

            /// Operators, column argument.
            ///@{
            View& operator =  ( const Column< T >& column );
            View& operator += ( const Column< T >& column );
            View& operator -= ( const Column< T >& column );
            View& operator *= ( const Column< T >& column );
            View& operator /= ( const Column< T >& column );
            bool  operator == ( const Column< T >& column ) const;
            bool  operator != ( const Column< T >& column ) const;
            ///@}

            /// Operators, row argument.
            ///@{
            View& operator =  ( const Row< T >& row );
            View& operator += ( const Row< T >& row );
            View& operator -= ( const Row< T >& row );
            View& operator *= ( const Row< T >& row );
            View& operator /= ( const Row< T >& row );
            bool  operator == ( const Row< T >& row ) const;
            bool  operator != ( const Row< T >& row ) const;
            ///@}

            /// Operators, scalar argument.
            ///@{
            View& operator =  ( const T scalar );
            View& operator += ( const T scalar );
            View& operator -= ( const T scalar );
            View& operator *= ( const T scalar );
            View& operator /= ( const T scalar );
            bool  operator == ( const T scalar ) const;
            bool  operator != ( const T scalar ) const;
            ///@}

        private :

            /// Constructor.
            View( const size_t ncols, const size_t nrows, const size_t stride, T* data );

            size_t  _ncols;     ///< Number of columns.
            size_t  _nrows;     ///< Number of rows.
            size_t  _stride;    ///< Number of columns of referenced image.
            T*      _data;      ///< Offset raw pixels of some image.

    };

    /// Stream output.
    template< typename T > std::ostream& operator << ( std::ostream& stream, const View< T >& view );

}

#include "C3_View.inl.hh"

#endif
