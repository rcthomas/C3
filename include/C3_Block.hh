#ifndef C3_BLOCK_HH
#define C3_BLOCK_HH

#include "C3.hh"

namespace C3
{

    /// @class Block
    /// @brief Native C++ array block interface.
    ///
    /// This template provides an interface to a native C++ array block (T*),
    /// intended to represent pixels of a given C++ data type.  It serves as
    /// underlying storage for C3 pixel containers.  Native C++ arrays are used
    /// instead of the other obvious alternatives for reasons described below.  
    /// 
    /// A very high-level requirement we have for C3 pixel containers is the
    /// need to acquire a native C++ array of data objects for MPI message
    /// payloads under the C MPI API.  Not having to create temporary buffers
    /// for these or otherwise rough-up existing standard container interfaces
    /// promotes efficiency, clarity, and prevents errors.
    ///
    /// Adapting the std::array template is not appropriate, as it fixes the
    /// size of the array at compile time.  While C3 assumes that we know a lot
    /// about our problem at compile time, having to know the exact size of
    /// every pixel container we encounter during processing ahead of time is
    /// too inconvenient.
    ///
    /// The std::vector template would work very well but its interface also
    /// includes features we should avoid or not encourage in defining pixel
    /// containers.  Specifically, we have little use for dynamically resizing
    /// pixel containers.  We could still use std::vector for the underlying
    /// storage, but we have observed slightly worse performance from
    /// std::vector for some simple tasks compared to native C++ arrays.
    ///
    /// The std::valarray template is interesting but seems to have variable
    /// performance characteristics depending on compiler.  The Intel compiler
    /// seems very good on this.  For some odd reason, it has no data() member
    /// like std::array and std::vector do.  The associated, non-member math
    /// functions look convenient but we would only use them very occasionally.
    /// Suggested implementations we have seen do a two pass (copy pass then
    /// replace pass) computation we avoid.
    ///
    /// Other types like std::list are not obviously good alternatives, so we
    /// do not bother considering them.  So native C++ array block it is.

    template< class T >
    class Block
    {

        public :    // Public methods.

            /// Array subscript access.
            ///@{
            T& operator [] ( const size_type pos )       { return _data[ pos ]; }
            T  operator [] ( const size_type pos ) const { return _data[ pos ]; }
            ///@}

            /// Total elements.
            size_type size() const { return _size; }

            /// Native C++ array access.
            ///@{
                  T* data()       { return _data; }
            const T* data() const { return _data; }
            ///@}

            /// Start of native C++ array.
            ///@{
                  T* begin()       { return data(); }
            const T* begin() const { return data(); }
            ///@}
           
            /// End of native C++ array.
            ///@{
                  T* end()       { return data() + size(); }
            const T* end() const { return data() + size(); }
            ///@}

        protected : // Protected methods.

            /// Constructor.
            Block( const size_type size, T* data ) noexcept :
                _size( size ), _data( data ) {}

            /// Destructor.
            ~Block() = default;

        protected : // Protected data members.

            size_type   _size;  ///< Total elements.
            T*          _data;  ///< Content.

    };

}

#endif
