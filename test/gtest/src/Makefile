# Makefile adapted from the Google test makefile.

CXX=CC

# Points to the root of Google Test, relative to where this file is.  Remember
# to tweak this if you move this file.

GTEST_DIR = ..

# Flags passed to the preprocessor.  Set Google Test's header directory as a
# system directory, such that the compiler doesn't generate warnings in Google
# Test headers.

CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -std=c++11 -g -Wall -Wextra -pthread

# All Google Test headers.  Usually you shouldn't change this definition.

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h

# Usually you shouldn't tweak such internal variables, indicated by a # trailing _.

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

GTEST_INSTALL=$(GTEST_DIR)/lib

# For simplicity and to avoid depending on Google Test's implementation
# details, the dependencies specified below are conservative and not optimized.
# This is fine as Google Test compiles fast and for ordinary users its source
# rarely changes.

TARGET=libgtest.a

all : $(TARGET)

$(TARGET) : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

install : $(TARGET)
	mkdir -p $(GTEST_INSTALL); cp $(TARGET) $(GTEST_INSTALL)/.

clean :
	rm -rf *.o

realclean : clean
	rm -rf $(TARGET)
