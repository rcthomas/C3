#ifndef C3_CONGRUENCE_HH
#define C3_CONGRUENCE_HH

namespace C3
{

    // Congruence tag types.
    
    struct PixelCongruence  {};
    struct RowCongruence    {};
    struct ColumnCongruence {};
    struct FrameCongruence  {};
    struct StackCongruence  {};
    
    /// @class CongruenceTraits
    /// @brief Maps container-type pairs to congruence tag types.
    ///
    /// This traits class returns a single type corresponding to one of the
    /// above congruence types, or UndefinedType if none applies.  Congruence
    /// types are used to select the appropriate implementation of congruence
    /// checks between containers.
    ///
    /// The primary template implementation governs pixel-vs-container checks
    /// (and container-vs-pixel checks) as PixelCongruence.  If no matching 
    /// specialization is found for a pair of types, the UndefinedType is 
    /// the result.
    
    template< class T, class U >
    struct CongruenceTraits
    {
        const bool container_and_pixel = ContainerPairTraits< T, U >::container_and_pixel;
        using type = std::conditional< container_and_pixel, PixelCongruence, UndefinedType >::type;
    };
    
    // Column source specializations.  All have row convergence but working out
    // the type traits for that takes up almost as many lines of code.
    
    template< class T, class U > struct CongruenceTraits< Column< T >, Column< U > > { using type = RowCongruence;    };
    template< class T, class U > struct CongruenceTraits<  Frame< T >, Column< U > > { using type = RowCongruence;    };
    template< class T, class U > struct CongruenceTraits<  Stack< T >, Column< U > > { using type = RowCongruence;    };
    template< class T, class U > struct CongruenceTraits<   View< T >, Column< U > > { using type = RowCongruence;    };
    
    // Row source specializations.
    
    template< class T, class U > struct CongruenceTraits<   Row< T >, Row< U > >     { using type = ColumnCongruence; };
    template< class T, class U > struct CongruenceTraits< Frame< T >, Row< U > >     { using type = ColumnCongruence; };
    template< class T, class U > struct CongruenceTraits< Stack< T >, Row< U > >     { using type = ColumnCongruence; };
    template< class T, class U > struct CongruenceTraits<  View< T >, Row< U > >     { using type = ColumnCongruence; };
    
    // Frame source specializations.
    
    template< class T, class U > struct CongruenceTraits< Column< T >, Frame< U > >  { using type = RowCongruence;    };
    template< class T, class U > struct CongruenceTraits<    Row< T >, Frame< U > >  { using type = ColumnCongruence; };
    template< class T, class U > struct CongruenceTraits<  Frame< T >, Frame< U > >  { using type = FrameCongruence;  };
    template< class T, class U > struct CongruenceTraits<  Stack< T >, Frame< U > >  { using type = FrameCongruence;  };
    template< class T, class U > struct CongruenceTraits<   View< T >, Frame< U > >  { using type = FrameCongruence;  };
    
    // Stack source specializations.
    
    template< class T, class U > struct CongruenceTraits< Column< T >, Stack< U > >  { using type = RowCongruence;    };
    template< class T, class U > struct CongruenceTraits<    Row< T >, Stack< U > >  { using type = ColumnCongruence; };
    template< class T, class U > struct CongruenceTraits<  Frame< T >, Stack< U > >  { using type = FrameCongruence;  };
    template< class T, class U > struct CongruenceTraits<  Stack< T >, Stack< U > >  { using type = StackCongruence;  };
    template< class T, class U > struct CongruenceTraits<   View< T >, Stack< U > >  { using type = FrameCongruence;  };
    
    // View source specializations.
    
    template< class T, class U > struct CongruenceTraits< Column< T >, View< U > >   { using type = RowCongruence;    };
    template< class T, class U > struct CongruenceTraits<    Row< T >, View< U > >   { using type = ColumnCongruence; };
    template< class T, class U > struct CongruenceTraits<  Frame< T >, View< U > >   { using type = FrameCongruence;  };
    template< class T, class U > struct CongruenceTraits<  Stack< T >, View< U > >   { using type = FrameCongruence;  };
    template< class T, class U > struct CongruenceTraits<   View< T >, View< U > >   { using type = FrameCongruence;  };
    
    // Congruence check implementations, selected via tag dispatch.
    
    template< class T, class U >
    inline bool congruent( const T& lhs, const U& rhs, UndefinedType )
    {
        return false;
    }
    
    template< class T, class U >
    inline bool congruent( const T& lhs, const U& rhs, PixelCongruence )
    {
        return true;
    }
    
    template< class T, class U >
    inline bool congruent( const T& lhs, const U& rhs, SizeCongruence )
    {
        return lhs.size() == rhs.size(); 
    }
    
    template< class T, class U >
    inline bool congruent( const T& lhs, const U& rhs, RowCongruence )
    {
        return lhs.nrows() == rhs.nrows(); 
    }
    
    template< class T, class U >
    inline bool congruent( const T& lhs, const U& rhs, ColumnCongruence )
    {
        return lhs.ncolumns() == rhs.ncolumns(); 
    }
    
    template< class T, class U >
    inline bool congruent( const T& lhs, const U& rhs, FrameCongruence )
    {
        return lhs.ncolumns() == rhs.ncolumns()
            && lhs.nrows()    == rhs.nrows();
    }
    
    template< class T, class U >
    inline bool congruent( const T& lhs, const U& rhs, StackCongruence )
    {
        return lhs.nframes()  == rhs.nframes()
            && lhs.ncolumns() == rhs.ncolumns()
            && lhs.nrows()    == rhs.nrows();
    }

}

#endif
