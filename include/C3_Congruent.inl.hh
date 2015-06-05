
template< class T, class U >
inline bool C3::congruent( const T& lhs, const U& rhs )
{
    typename OperationTraits< T, U >::congruence_type congruence;
    return C3::Detail::congruent( lhs, rhs, congruence );
}

template< class T, class U >
inline bool C3::Detail::congruent( const T& lhs, const U& rhs, C3::NullType )
{
    return false;
}

template< class T, class U >
inline bool C3::Detail::congruent( const T& lhs, const U& rhs, C3::PixelCongruence )
{
    return true;
}

template< class T, class U >
inline bool C3::Detail::congruent( const T& lhs, const U& rhs, C3::SizeCongruence )
{
    return lhs.size() == rhs.size(); 
}

template< class T, class U >
inline bool C3::Detail::congruent( const T& lhs, const U& rhs, C3::RowCongruence )
{
    return lhs.nrows() == rhs.nrows(); 
}

template< class T, class U >
inline bool C3::Detail::congruent( const T& lhs, const U& rhs, C3::ColumnCongruence )
{
    return lhs.ncolumns() == rhs.ncolumns(); 
}

template< class T, class U >
inline bool C3::Detail::congruent( const T& lhs, const U& rhs, C3::FrameCongruence )
{
    return lhs.ncolumns() == rhs.ncolumns()
        && lhs.nrows()    == rhs.nrows();
}

template< class T, class U >
inline bool C3::Detail::congruent( const T& lhs, const U& rhs, C3::StackCongruence )
{
    return lhs.nframes()  == rhs.nframes()
        && lhs.ncolumns() == rhs.ncolumns()
        && lhs.nrows()    == rhs.nrows();
}
