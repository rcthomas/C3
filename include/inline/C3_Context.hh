
// Initialization access point.

template< template< class > class Concurrency, class InstrumentTraits >
inline C3::Context< Concurrency, InstrumentTraits >& C3::Context< Concurrency, InstrumentTraits >::instance( int& argc, char**& argv )
{
    C3::Context< Concurrency, InstrumentTraits >& context = C3::Context< Concurrency, InstrumentTraits >::instance();
    context.init( argc, argv );
    return context;
}

// Access point after initialization.

template< template< class > class Concurrency, class InstrumentTraits >
inline C3::Context< Concurrency, InstrumentTraits >& C3::Context< Concurrency, InstrumentTraits >::instance()
{
    static C3::Context< Concurrency, InstrumentTraits > context;
    return context;
}
