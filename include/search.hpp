#pragma once

#include "types.hpp"

int search(Position& pos, int depth, int ply, U64& nodes);
SearchResult findBestMove(Position& pos, int depth);