#include "../include/types.hpp"
#include "../include/movegen.hpp"
#include <vector>
using namespace std;

void addSlidingMoves(const Position& pos, int from, vector<Move>& moves);
int getPieceType(Piece piece);
bool isSameColour(int colour, Piece targetPiece);

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

void addSlidingMoves(const Position& pos, int from, vector<Move>& moves) {
    const Piece *board = pos.pieces;
    int piece = getPieceType(board[from]);
    if (piece == NONE || !isSlidingPiece[piece]) return;
    int n = numDirections[piece];
    int colour = pos.sideToMove;
    for (int i = 0; i < n; i++) {
        int direction = moveDirections[piece][i];
        for (int j = 1; j < 8; j++) {
            int targetSquare = from + j * direction;
            int targetPiece = board[targetSquare];

            if (targetPiece == OFFBOARD) break;

            if (targetPiece == EMPTY) {
                moves.push_back({from, targetSquare});
                continue;
            }
            if (isSameColour(colour, targetPiece)) {
                break;
            } else {
                moves.push_back({from, targetSquare});
                break;
            }
        }
    }
}

int getPieceType(Piece piece) {
    if (piece == EMPTY) return NONE;
    return piece % 6;
}

bool isSameColour(int colour, Piece targetPiece) {
    if (colour == WHITE && (targetPiece >= wP && targetPiece <= wK)) return true;
    if (colour == BLACK && (targetPiece >= bP && targetPiece <= bK)) return true;
    return false;
}

// Focusing on only pseudo-legal moves currently
vector<Move> generateMoves(const Position& pos) {
    vector<Move> moves;
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;

    for (int i = 0; i < NUM_SQUARES; i++) {
        if (board[i] == OFFBOARD || board[i] == EMPTY || !isSameColour(colour, board[i])) continue;
        addSlidingMoves(pos, i, moves);
    }

    return moves;
}