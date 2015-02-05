#ifndef C3__STACK
#define C3__STACK

namespace C3
{

    template< typename T > struct Column;
    template< typename T > struct Image;
    template< typename T > struct Row;
    template< typename T > struct View;

    /// @class Stack
    /// @brief Fixed-size stack of images.

    template< typename T >
    struct Stack
    {

        /// Pixel type.
        typedef T value_type;

        /// Create stack.
        static Stack create( const size_t nimgs, const size_t ncols, const size_t nrows );

        /// Create stack and initialize with a scalar value.
        static Stack create( const size_t nimgs, const size_t ncols, const size_t nrows, const T scalar );

        /// Constructor.
        Stack( const size_t nimgs, const size_t ncols, const size_t nrows, T* data );

        /// Constructor, copy.
        Stack( const Stack& stack );

        /// Destructor.
        ~Stack();

        /// Size, shape.
        ///@{
        size_t nimgs() const { return _nimgs; }
        size_t ncols() const { return _ncols; }
        size_t nrows() const { return _nrows; }
        size_t size()  const { return _nimgs * _ncols * _nrows; }
        ///@}

        /// Access.
        ///@{
        T  operator () ( const size_t i, const size_t j, const size_t k ) const { return _data[ i + _ncols * ( j + _nrows * k ) ]; }
        T& operator () ( const size_t i, const size_t j, const size_t k )       { return _data[ i + _ncols * ( j + _nrows * k ) ]; }
        ///@}

        /// Operators, stack argument.
        ///@{
        Stack& operator =  ( const Stack& stack );
        Stack& operator += ( const Stack& stack );
        Stack& operator -= ( const Stack& stack );
        Stack& operator *= ( const Stack& stack );
        Stack& operator /= ( const Stack& stack );
        bool   operator == ( const Stack& stack ) const;
        bool   operator != ( const Stack& stack ) const;
        ///@}

        /// Cast to stack of another type.
        template< typename U > operator Stack< U > () const;

        // Data members.

        size_t _nimgs; ///< Number of images.
        size_t _ncols; ///< Number of columns.
        size_t _nrows; ///< Number of rows.
        T*     _data;  ///< Raw pixels.

    };

    /// Stream output.
    template< typename T > std::ostream& operator << ( std::ostream& stream, const Stack< T >& stack );

}

#include "C3_Stack.inl.hh"

#endif
