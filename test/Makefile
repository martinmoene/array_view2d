# Copyright 2014 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# array_view2d is inspired on https://github.com/rhysd/array_view by Linda_pp.

# Usage: gmake [STD=c++03]

PROGRAM = array_view2d.t
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

ifdef STD
STD_OPTION = -std=$(STD)
endif

CXX = g++
CXXFLAGS = $(STD_OPTION) -Wall -Dlest_MAIN -Dav_FEATURE_EXPECT_THROWS=1 -I../include/ # -Wextra 

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $@ $^

test: $(PROGRAM)
	./$(PROGRAM)

test-all: $(PROGRAM)
	./$(PROGRAM) @

list: test
	./$(PROGRAM) -l

clean:
	$(RM) $(OBJECTS)
	$(RM) $(PROGRAM)

