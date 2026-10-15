#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <vector>
using namespace std;

extern bool isSlidingPiece[6];
extern int numDirections[6];
extern int moveDirections[6][8];

// Use this enum for indexing in numDirections and moveDirections
enum {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NONE
};

vector<Move> generateMoves(const Position& pos);

#endif