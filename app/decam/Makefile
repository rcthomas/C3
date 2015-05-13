
TARGET = \
    decam-preprocess-science

# I hope we can use the Intel compiler at NERSC to get the best performance.

CXX=CC
CXXFLAGS+=-std=c++11 -qopenmp -Wno-deprecated

# Need boost for yaml-cpp; NERSC module.  Developer is factoring out the boost
# dependency.

INCLUDE+=-I$(BOOST_DIR)/include

# Need CFITSIO; NERSC module.

INCLUDE+=-I$(CFITSIO_DIR)/include
LDFLAGS+=-L$(CFITSIO_DIR)/lib
LIBS+=-lcfitsio

# Need C3 headers and yaml-cpp.  Source incorporated for now but may change.
# CMAKE install on NERSC systems requires attention (not I said the cat).

C3_DIR=../..
INCLUDE+=-I$(C3_DIR)/external/yaml-cpp/include
LDFLAGS+=-L$(C3_DIR)/external/yaml-cpp/src
LIBS+=-lyaml-cpp

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