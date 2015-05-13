
TARGET=libyaml-cpp.a

OBJECTS=binary.o convert.o directives.o emit.o emitfromevents.o emitter.o     \
        emitterstate.o emitterutils.o exp.o memory.o node.o node_data.o       \
        nodebuilder.o nodeevents.o null.o ostream_wrapper.o parse.o parser.o  \
        regex_yaml.o scanner.o scanscalar.o scantag.o scantoken.o simplekey.o \
        singledocparser.o stream.o tag.o

# Requires boost for now, but there is work to factor that out and use only
# C++11 constructs.  Here for consistency I have enabled the C++11 standard
# flag, which necessitated a few changes here and here in the source (nullptr
# and the like).

INCLUDE=-I$(BOOST_DIR)/include -I../include

CXX=CC
CXXFLAGS=-std=c++11 -fast $(INCLUDE) -Wno-deprecated
LDFLAGS=-L.
LDLIBS=-lyaml-cpp

all: $(TARGET)

libyaml-cpp.a : $(OBJECTS)
	ar rcs $@ $^

.cpp.o :
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf *.o

realclean: clean
	rm -rf $(TARGET) core.*
