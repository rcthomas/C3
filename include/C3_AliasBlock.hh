#ifndef C3_ALIAS_BLOCK_HH
#define C3_ALIAS_BLOCK_HH

#include "C3_Block.hh"

namespace C3
{

    /// @class AliasBlock
    /// @brief Block that shadows another block.

    template< class T >
    class AliasBlock : public Block< T >
    {

        public :    // Public methods.

            /// Constructor.
            explicit AliasBlock( Block< T >& block ) noexcept :
                Block< T >( block.size(), block.data() ) {}

    };

}

#endif
