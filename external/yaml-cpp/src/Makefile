
CXX=CC

TARGET=try

OBJECTS=binary.o convert.o directives.o emit.o emitfromevents.o emitter.o emitterstate.o emitterutils.o exp.o memory.o node.o node_data.o nodebuilder.o nodeevents.o null.o ostream_wrapper.o parse.o parser.o regex_yaml.o scanner.o scanscalar.o scantag.o scantoken.o simplekey.o singledocparser.o stream.o tag.o

INCLUDE=-I$(BOOST_DIR)/include -I../include

all: $(TARGET)

$(TARGET) : try.o libyaml-cpp.a
	$(CXX) -L. -o try try.o -lyaml-cpp

libyaml-cpp.a : $(OBJECTS)
	ar rcs $@ $^

.cpp.o :
	$(CXX) -O3 $(INCLUDE) -c -o $@ $<

clean:
	rm -rf *.o

realclean: clean
	rm -rf $(TARGET) core.*
