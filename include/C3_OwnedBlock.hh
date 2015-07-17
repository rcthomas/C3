#ifndef C3_OWNED_BLOCK_HH
#define C3_OWNED_BLOCK_HH

#include "C3_Block.hh"

namespace C3
{

    /// @class OwnedBlock
    /// @brief Block with memory management.

    template< class T >
    class OwnedBlock : public Block< T >
    {

        public :    // Public methods.

            /// Constructor.
            explicit OwnedBlock( const size_type size ) noexcept;

            /// Copy constructor.
            OwnedBlock( const OwnedBlock& block ) noexcept;

            /// Move constructor.
            OwnedBlock( OwnedBlock&& block ) noexcept;

            /// Copy assignment.
            OwnedBlock& operator = ( const OwnedBlock& block ) noexcept;

            /// Move assignment.
            OwnedBlock& operator = ( OwnedBlock&& block ) noexcept;

            /// Destructor.
            ~OwnedBlock();

    };

}

#include "inline/C3_OwnedBlock.hh"

#endif
