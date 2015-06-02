#ifndef C3_ALIAS_BLOCK_HH
#define C3_ALIAS_BLOCK_HH

#include "C3_Block.hh"

namespace C3
{

    template< class T >
    class AliasBlock : public Block< AliasBlock< T > >
    {

        public :    // Public type definitions.

            using super_type = Block< AliasBlock< T > >;
            using size_type  = typename super_type::size_type;  ///< Index and size type.
            using value_type = typename super_type::value_type; ///< Content type.

            friend super_type;  ///< Grant protected access to super-class.

        public :    // Public methods.

            /// Constructor.
            explicit AliasBlock( Block< T >& block ) noexcept :
                super_type( block.size(), block.data() ) {}

    };

}

#endif
