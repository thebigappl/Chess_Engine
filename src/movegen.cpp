#include "../include/types.hpp"
#include "../include/movegen.hpp"
#include <vector>
using namespace std;

void addSlidingMoves(const Position& pos, int from, int pieceType, vector<Move>& moves);
int getPieceType(Piece piece);
bool isSameColour(int colour, Piece targetPiece);
void addKnightMoves(const Position& pos, int from, vector<Move>& moves);
void addPawnMoves(const Position& pos, int from, vector<Move>& moves);

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

// Assumes `from` contains a sliding piece of the side to move.
void addSlidingMoves(const Position& pos, int from, int pieceType, vector<Move>& moves) {
    // Sliding move generation only
    const Piece *board = pos.pieces;
    int n = numDirections[pieceType];
    int colour = pos.sideToMove;
    for (int i = 0; i < n; i++) {
        int direction = moveDirections[pieceType][i];
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

// Gets the general pieceType (PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING or NONE)
int getPieceType(Piece piece) {
    if (piece == EMPTY) return NONE;
    return piece % 6;
}

// Checks if the targetPiece is of the same colour as the side to move
bool isSameColour(int colour, Piece targetPiece) {
    if (colour == WHITE && (targetPiece >= wP && targetPiece <= wK)) return true;
    if (colour == BLACK && (targetPiece >= bP && targetPiece <= bK)) return true;
    return false;
}

void addKnightMoves(const Position& pos, int from, vector<Move>& moves) {
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    int n = numDirections[KNIGHT];
    for (int i = 0; i < n; i++) {
        int targetSquare = from + moveDirections[KNIGHT][i];
        int targetPiece = board[targetSquare];

        if (targetPiece == OFFBOARD) continue;
        if (targetPiece == EMPTY || !isSameColour(colour, targetPiece)) moves.push_back({from, targetSquare});
    }
}

// Pawn moves is WIP, contains basically everything aside from promotions:
// - normal pawn movement into promotion
// - capture into promotion
void addPawnMoves(const Position& pos, int from, vector<Move>& moves) {
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    int enPassantSquare = pos.enPassantSquare;

    // First check normal pawn movement
    int targetSquare = from - 10;
    int targetPiece = board[targetSquare];
    if (targetPiece == EMPTY) moves.push_back({from, targetSquare});

    // Check pawn captures
    // Left capture
    targetSquare = from - 11;
    targetPiece = board[targetSquare];
    if ((targetPiece != EMPTY && targetPiece != OFFBOARD && !isSameColour(colour, targetPiece)) ||
        from - 1 == enPassantSquare) {
        moves.push_back({from, targetSquare});
    }

    // Right capture
    targetSquare = from - 9;
    targetPiece = board[targetSquare];
    if ((targetPiece != EMPTY && targetPiece != OFFBOARD && !isSameColour(colour, targetPiece)) ||
        from + 1 == enPassantSquare) {
        moves.push_back({from, targetSquare});
    }
}


// Focusing on only pseudo-legal moves currently
vector<Move> generateMoves(const Position& pos) {
    vector<Move> moves;
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;

    for (int i = 0; i < NUM_SQUARES; i++) {
        Piece piece = board[i];
        if (piece == OFFBOARD || piece == EMPTY) continue;
        if (!isSameColour(colour, piece)) continue;

        int pieceType = getPieceType(piece);
        if (isSlidingPiece[pieceType]) {
            addSlidingMoves(pos, i, pieceType, moves);
        } else if (pieceType == KNIGHT) {
            addKnightMoves(pos, i, moves);
        } else if (pieceType == PAWN) {
            // addPawnMoves(...)
        }
        // king moves, etc...
    }

    return moves;
}