#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

bool isSlidingPiece[6] = {false, false, true, true, true, false};
int numDirections[6] = {0, 8, 4, 4, 8, 8};
int moveDirections[6][8] = {
    {   0,   0,  0,  0, 0,  0,  0,  0 },
    { -21, -19,-12, -8, 8, 12, 19, 21 }, // Knight
    { -11,  -9,  9, 11, 0,  0,  0,  0 }, // Bishop
    { -10,  -1,  1, 10, 0,  0,  0,  0 }, // Rook
    { -11, -10, -9, -1, 1,  9, 10, 11 }, // Queen
    { -11, -10, -9, -1, 1,  9, 10, 11 }  // King
};

// Use this enum for indexing in numDirections and moveDirections
enum {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

#endif