#!/usr/bin/env python

import  os

class Makefile ( object ) :

    def __init__( self, cxx, cxxflags ) :
        self.cxx      = cxx
        self.cxxflags = cxxflags

    def __repr__( self ) :
        output =  "# {}\n\n".format( self.__class__.__name__ )
        output += "{:10} = {}\n".format( "CXX"       , self.cxx      )
        output += "{:10} = {}\n".format( "CXXFLAGS"  , self.cxxflags )
        output += "\n"
        output += "all :\n\n"
        output += "test :\n"
        output += "\tcd testing && make test\n\n"
        output += "distclean :\n"
        output += "\tcd app/decam && make realclean\n"
        output += "\tcd testing && make deepclean\n"
        return output

class EdisonMakefile ( Makefile ) :

    @classmethod
    def is_match( cls ) :
        return os.environ.get( "NERSC_HOST", "" ) == "edison"

    def __init__( self, cxx = None, cxxflags = None ) :
        cxx      = cxx      or "CC"
        cxxflags = cxxflags or "-std=c++11 -fast -no-ipo"
        super( EdisonMakefile, self ).__init__( cxx, cxxflags )

class ClangMakefile ( Makefile ) :

    @classmethod
    def is_match( cls ) :
        uname = os.uname()
        return uname[ 0 ].lower() == "darwin"

    def __init__( self, cxx = None, cxxflags = None ) :
        cxx      = cxx      or "c++"
        cxxflags = cxxflags or "-std=c++11 -O3"
        super( ClangMakefile, self ).__init__( cxx, cxxflags )

class OtherMakefile ( Makefile ) :

    @classmethod
    def is_match( cls ) :
        uname = os.uname()
        return uname[ 0 ].lower() == "darwin"

    def __init__( self, cxx = None, cxxflags = None ) :
        import pprint
        pprint.pprint( os.environ )
        cxx      = cxx      or "clang"
        cxxflags = cxxflags or "-std=c++11 -O3"
        super( OtherMakefile, self ).__init__( cxx, cxxflags )

class MakefileFactory ( object ) :

    def __init__( self, **kwargs ) :
        self.registry = kwargs

    def create( self, makefile_type = None, **kwargs ) :
        if makefile_type is None :
            for key, makefile_class in self.registry.iteritems() :
                if not makefile_class.is_match() :
                    continue
                makefile_type = key
        if makefile_type is None :
#           raise NotImplementedError( "unrecognized environment" )
            makefile_type = "other"
        makefile_class = self.registry[ makefile_type ]
        return makefile_class( kwargs )

if __name__ == "__main__" :

    factory  = MakefileFactory( edison = EdisonMakefile, clang = ClangMakefile, other = OtherMakefile )
    makefile = factory.create()
    with open( "Makefile", "w" ) as stream :
        stream.write( "{}".format( makefile ) )

