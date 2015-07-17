
#include "../C3_Block.hh"
#include "../C3_FitsException.hh"
#include "../C3_FitsTraits.hh"

template< class T > 
void C3::fits_create_one( const C3::Block< T >& block, const std::string& path )
{} 

C3::FitsCreator::FitsCreator( const std::string& path ) :
    C3::FitsResource( path, READWRITE )
{}

template< class T > 
void C3::FitsCreator::operator() ( const Block< T >& block, const std::string& extname )
{
    select( extname );
    save( block );
}

void C3::FitsCreator::select( const std::string& extname )
{ /* TBD */ }

template< class T >
void C3::FitsCreator::save( const C3::Block< T >& block )
{ /* TBD */ }
