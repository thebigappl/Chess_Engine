CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
CPPFLAGS = -Iinclude

ENGINE_SRC = src/main.cpp src/movegen.cpp src/position.cpp
TEST_SRC = tests/test_movegen.cpp src/movegen.cpp src/position.cpp

.PHONY: all test clean

all: chess

chess: $(ENGINE_SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(ENGINE_SRC) -o chess

test: test_movegen
	./test_movegen

test_movegen: $(TEST_SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_SRC) -o test_movegen

clean:
	rm -f chess test_movegen
