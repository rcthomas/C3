
include ../Makefile

# Google Test.

GTEST_DIR=./gtest
CXXFLAGS += -I$(GTEST_DIR)/include -pthread -fno-elide-constructors
LDFLAGS  += -L$(GTEST_DIR)/lib
LIBS     += -lgtest -lpthread

# C3

C3_DIR=..
CXXFLAGS += -I$(C3_DIR)/include

#

TARGET=test-c3
OBJECTS=$(patsubst %-test.cc,%-test.o,$(wildcard *-test.cc)) $(TARGET).o

all : $(TARGET)

$(TARGET) : gtest $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

gtest : $(GTEST_DIR)/lib/libgtest.a

$(GTEST_DIR)/lib/libgtest.a :
	cd $(GTEST_DIR) && make all

.cc.o :
	$(CXX) $(CXXFLAGS) -c $< -o $@    

clean :
	rm -rf *.o

realclean : clean
	rm -rf $(TARGETS) core.*

deepclean : realclean
	cd gtest && make realclean
