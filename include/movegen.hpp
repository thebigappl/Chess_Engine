#pragma once

#include <vector>
#include "types.hpp"
using namespace std;

MoveList generateMoves(const Position& pos);
MoveList generateLegalMoves(const Position& pos);
