#ifndef C3_OWNED_BLOCK_HH
#define C3_OWNED_BLOCK_HH

#include "C3_Block.hh"

namespace C3
{

    template< class T >
    class OwnedBlock : public Block< OwnedBlock< T > >
    {

        public :    // Public type definitions.

            using super_type = Block< OwnedBlock< T > >;
            using size_type  = typename super_type::size_type;  ///< Index, size type.
            using value_type = typename super_type::value_type; ///< Content type.

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

#include "C3_OwnedBlock.inl.hh"

#endif
