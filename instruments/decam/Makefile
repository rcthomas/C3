
# TARGET =                            \
#     decam-preprocess-science

TARGET =    \
    decam-read-test-serial      \
    decam-read-test-parallel

# CXX=g++ -std=c++11 -Wno-deprecated -O3 -funroll-loops -ffast-math
# CXX=mpicxx -std=c++11 -Wno-deprecated -O3 -funroll-loops -ffast-math
# CXX=CC -std=c++11 -O3 -static-libgcc # -Wno-deprecated -O3 -funroll-loops -ffast-math -static-libgcc
CXX=CC -std=c++11 -O3# -static-libgcc # -Wno-deprecated -O3 -funroll-loops -ffast-math -static-libgcc

CFITSIO_INCLUDE=-I$(CFITSIO_DIR)/include
CFITSIO_LDFLAGS=-L$(CFITSIO_DIR)/lib
CFITSIO_LIBS=-lcfitsio

INCLUDE=$(CFITSIO_INCLUDE) -I../../include
LDFLAGS=$(CFITSIO_LDFLAGS)
LIBS=$(CFITSIO_LIBS)

# OBJ=                    \
#     DECam_Chip.o

#    DECam_FocalPlane.o 

OBJECT=$(OBJ) $(TARGET:=.o)

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CXX) $(LDFLAGS) $(OBJ) $@.o -o $@ $(LIBS)

.cc.o :
	$(CXX) $(INCLUDE) -c -o $@ $<

clean:
	rm -rf *.o

realclean: clean
	rm -rf $(TARGET) core.*
