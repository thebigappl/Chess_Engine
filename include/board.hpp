#pragma once

#include "types.hpp"

int getRank(Square square);
int getFile(Square square);
bool isStartingPosition(int colour, int rank);
bool isPromotionRank(int colour, int rank);
int getM64Index(Square square);