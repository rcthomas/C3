#ifndef C3__COMMUNICATOR
#define C3__COMMUNICATOR

#include "C3_MPI.hh"

namespace C3
{

    /// @class Communicator
    /// @brief Wrapper for opaque MPI communicator object.

    class Communicator
    {

        public :
        
            /// Constructor.
            Communicator( MPI_Comm comm );
        
            /// Copy constructor.
            Communicator( const Communicator& comm );
        
            /// Destructor.
            ~Communicator();
        
            /// Assignment.
            Communicator& operator = ( const Communicator& comm );
        
            /// Underlying opaque MPI communicator.
            MPI_Comm comm() const { return _comm; }
        
            /// Number of MPI processes in communicator.
            int size() const { return _size; }
        
            /// Rank of this MPI process.
            int rank() const { return _rank; }
        
            /// True if this MPI process has rank zero.
            bool root() const { return _root; }
        
        private :
        
            MPI_Comm    _comm;  ///< Underlying opaque MPI communicator object.
            int         _size;  ///< Number of MPI processes in communicator.
            int         _rank;  ///< Rank of this MPI process.
            bool        _root;  ///< True if this MPI process has rank zero.

    };

}

#include "C3_Communicator.inl.hh"

#endif
