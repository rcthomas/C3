#ifndef C3_ALIAS_BLOCK_HH
#define C3_ALIAS_BLOCK_HH

#include "C3_Block.hh"

namespace C3
{

    template< class T >
    class AliasBlock : public Block< T >
    {

        public :    // Public type definitions.

            using super_type = Block< T >;  ///< Super-class type.

        public :    // Public methods.

            /// Constructor.
            explicit AliasBlock( Block< T >& block ) noexcept :
                super_type( block.size(), block.data() ) {}

    };

}

#endif
