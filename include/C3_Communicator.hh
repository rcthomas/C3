#ifndef C3_COMMUNICATOR_HH
#define C3_COMMUNICATOR_HH

#include <mpi.h>

namespace C3
{

    /// @class Communicator
    /// @brief Wrapper for opaque MPI communicator object.

    class Communicator
    {

        public :    // Public methods.
        
            /// Constructor.
            explicit Communicator( MPI_Comm comm );
        
            /// Underlying opaque MPI communicator.
            MPI_Comm comm() const { return _comm; }
        
            /// Number of MPI processes in communicator.
            int size() const { return _size; }
        
            /// Rank of this MPI process.
            int rank() const { return _rank; }
        
            /// True if this MPI process has rank zero.
            bool root() const { return _root; }
        
        private :   // Private data members.
        
            MPI_Comm    _comm;  ///< Underlying opaque MPI communicator object.
            int         _size;  ///< Number of MPI processes in communicator.
            int         _rank;  ///< Rank of this MPI process.
            bool        _root;  ///< True if this MPI process has rank zero.

    };

}

#include "inline/C3_Communicator.hh"

#endif
