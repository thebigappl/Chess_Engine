#pragma once

#include <vector>
#include "types.hpp"
using namespace std;

vector<Move> generateMoves(const Position& pos);
vector<Move> generateLegalMoves(const Position& pos);
