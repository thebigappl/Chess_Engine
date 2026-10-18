#pragma once

#include "types.hpp"

// movegen
inline constexpr bool isSlidingPiece[6] = {false, false, true, true, true, false};
inline constexpr int numDirections[6] = {0, 8, 4, 4, 8, 8};
inline constexpr int moveDirections[6][8] = {
    {   0,   0,  0,  0, 0,  0,  0,  0 },
    { -21, -19,-12, -8, 8, 12, 19, 21 }, // Knight
    { -11,  -9,  9, 11, 0,  0,  0,  0 }, // Bishop
    { -10,  -1,  1, 10, 0,  0,  0,  0 }, // Rook
    { -11, -10, -9, -1, 1,  9, 10, 11 }, // Queen
    { -11, -10, -9, -1, 1,  9, 10, 11 }  // King
};
inline constexpr int pawnDirections[2] = {1, -1};
inline constexpr int captureDirections[2] = {11, 9};
inline constexpr int castlingDirections[2] = {1, -1};
inline constexpr int originalRookPositions[4] = {H1, A1, H8, A8};

// eval + search
inline constexpr int pieceValues[6] = {100, 300, 300, 500, 900, 0};
inline constexpr int MATE_SCORE = 100000;
inline constexpr int INFINITY_SCORE = 1000000;