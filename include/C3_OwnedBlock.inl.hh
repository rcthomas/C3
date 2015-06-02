
#include "C3_Assign.hh"

// Constructor.

template< class T >
inline C3::OwnedBlock< T >::OwnedBlock( const size_type size ) noexcept :
    super_type( size, new value_type [ size ] )
{}

// Copy constructor.

template< class T >
inline C3::OwnedBlock< T >::OwnedBlock( const OwnedBlock< T >& block ) noexcept :
    super_type( block.size(), new value_type [ block.size() ] )
{
    C3::assign( *this, block );
}

// Move constructor.

template< class T >
inline C3::OwnedBlock< T >::OwnedBlock( OwnedBlock< T >&& block ) noexcept :
    super_type( block.size(), block.data() )
{
    block._size = 0;
    block._data = nullptr;
}

// Copy assignment.  Copying pixels from a block with one size to a block with
// a different size has undefined behavior with probably bad consequences.
// When C3 applications are compiled with the DEBUG flag, such a condition
// breaks an assertion.

template< class T >
inline C3::OwnedBlock< T >& C3::OwnedBlock< T >::operator = ( const C3::OwnedBlock< T >& block ) noexcept
{
    return this != &block ? C3::assign( *this, block ) : *this;
}

// Move assignment.

template< class T >
inline C3::OwnedBlock< T >& C3::OwnedBlock< T >::operator = ( C3::OwnedBlock< T >&& block ) noexcept
{
    if( this != &block )
    {
        this->_size = block._size;
        this->_data = block._data;
        block._size = 0;
        block._data = nullptr;
    }
    return *this;
}

// Destructor.

template< class T >
inline C3::OwnedBlock< T >::~OwnedBlock()
{
    delete [] this->_data;
}
