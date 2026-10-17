CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
CPPFLAGS = -Iinclude

ENGINE_SRC = src/main.cpp src/movegen.cpp src/position.cpp
TEST_COMMON_SRC = src/movegen.cpp src/position.cpp src/board.cpp  src/pieces.cpp src/attacks.cpp
TEST_MOVEGEN_SRC = tests/test_movegen.cpp $(TEST_COMMON_SRC)
TEST_MAKE_MOVE_SRC = tests/test_make_move.cpp src/move.cpp $(TEST_COMMON_SRC)

.PHONY: all test clean

all: chess

chess: $(ENGINE_SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(ENGINE_SRC) -o chess

test: test_movegen test_make_move
	./test_movegen
	./test_make_move

test_movegen: $(TEST_MOVEGEN_SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_MOVEGEN_SRC) -o test_movegen

test_make_move: $(TEST_MAKE_MOVE_SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_MAKE_MOVE_SRC) -o test_make_move

clean:
	rm -f chess test_movegen test_make_move