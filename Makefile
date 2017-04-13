CXX = g++
CXXFLAGS = -Wall -O2 -pedantic
OPTS = -MMD -MP -Iinclude

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%, obj/%, $(SRC:.cpp=.o))

TESTSRC := $(wildcard test/*.cpp)
TESTOBJ := $(patsubst test/%.cpp, test/%.o, $(TESTSRC:.cpp=.o))

all: main test

main: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o main

test: $(TESTOBJ) $(filter-out obj/main.o, $(OBJ))
	$(CXX) $(CXXFLAGS) $^ -o test/test-main
	test/test-main -r compact

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(OPTS) -c $< -o $@

test/%.o: test/%.cpp
	$(CXX) $(CXXFLAGS) $(OPTS) -c $< -o $@

clean:
	rm -f obj/* main test/test-main test/*.o test/*.d

-include $(OBJ:%.o=%.d)
-include $(TESTOBJ:%.o=%.d)

.PHONY: clean
