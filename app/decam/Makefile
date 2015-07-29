
TARGET = \
    decam-test-engines

include ../../Makefile
CXXFLAGS+=-std=c++11
#-qopenmp -Wno-deprecated

# Need boost for yaml-cpp; NERSC module.  Developer is factoring out the boost
# dependency.

INCLUDE+=-I$(BOOST_DIR)/include

# Need C3 headers and yaml-cpp.  Source incorporated for now but may change.
# CMAKE install on NERSC systems requires attention (not I said the cat).

C3_DIR=../..
INCLUDE+=-I$(C3_DIR)/include -I$(C3_DIR)/external/yaml-cpp/include
LDFLAGS+=-L$(C3_DIR)/external/yaml-cpp/src
LIBS+=-lyaml-cpp

# Need CFITSIO; NERSC module.

INCLUDE+=-I$(CFITSIO_DIR)/include
LDFLAGS+=-L$(CFITSIO_DIR)/lib
LIBS+=-lcfitsio

# Need GSL; NERSC module.

INCLUDE+=-I$(GSL_DIR)/include
LDFLAGS+=-L$(GSL_DIR)/lib
LIBS+=-lgsl

# 

OBJECT=$(TARGET:=.o)

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) $@.o -o $@ $(LIBS)

.cc.o :
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	rm -rf *.o

realclean: clean
	rm -rf $(TARGET) core.*
