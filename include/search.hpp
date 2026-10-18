#pragma once

#include "types.hpp"

int negamax(Position& pos, int depth);
Move findBestMove(Position& pos, int depth);