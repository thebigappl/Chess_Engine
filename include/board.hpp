#pragma once

#include "types.hpp"

int getRank(int square);
int getFile(int square);
bool isStartingPosition(int colour, int rank);
bool isPromotionRank(int colour, int rank);