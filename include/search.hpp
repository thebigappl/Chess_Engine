#pragma once

#include "types.hpp"

int search(Position& pos, int depth, int ply, U64& nodes, int alpha, int beta);
SearchResult findBestMove(Position& pos, int depth);