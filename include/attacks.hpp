#pragma once

#include "types.hpp"

bool isDiagonalDirection(int direction);

// Position validation functions
bool isPawnAttack(const Position& pos, int square, int attackingColour);
bool isKnightAttack(const Position& pos, int square, int attackingColour);
bool isSlidingAttack(const Position& pos, int square, int attackingColour);
bool isKingAttack(const Position& pos, int square, int attackingColour);
bool isSquareAttacked(const Position& pos, int square, int attackingColour);