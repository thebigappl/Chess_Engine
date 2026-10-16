#include "../include/types.hpp"
#include "../include/movegen.hpp"
#include <vector>
using namespace std;

void addSlidingMoves(const Position& pos, int from, int pieceType, vector<Move>& moves);
int getPieceType(Piece piece);
bool isSameColour(int colour, Piece targetPiece);
void addKnightMoves(const Position& pos, int from, vector<Move>& moves);
void addPawnMoves(const Position& pos, int from, vector<Move>& moves);
void addPawnPromotionMoves(Piece pawn, int from, int targetSquare, vector<Move>& moves);
int getRank(int square);
int getFile(int square);
bool isStartingPosition(int colour, int rank);
bool isPromotionRank(int colour, int rank);

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
int pawnDirections[2] = {1, -1};
int captureDirections[2] = {11, 9};
int castlingDirections[2] = {1, -1};

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

int getRank(int square) {
    return (square - A1) / 10;
}

int getFile(int square) {
    return (square - 1) % 10;
}

bool isStartingPosition(int colour, int rank) {
    return ((colour == WHITE && rank == RANK_2) ||
            (colour == BLACK && rank == RANK_7));
}

bool isPromotionRank(int colour, int rank) {
    return ((colour == WHITE && rank == RANK_8) || 
            (colour == BLACK && rank == RANK_1));
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
    int fromRank = getRank(from);
    int pawnDirection = pawnDirections[colour];
    int forwardDirection = pawnDirection * 10;

    // First check normal pawn movement
    int targetSquare = from + forwardDirection;
    int targetRank = getRank(targetSquare);
    int targetPiece = board[targetSquare];
    if (targetPiece == EMPTY) {
        if (isPromotionRank(colour, targetRank)) {
            addPawnPromotionMoves(board[from], from, targetSquare, moves);
        } else {
            moves.push_back({from, targetSquare});
        }

        if (isStartingPosition(colour, fromRank)) {
            int doubleTarget = targetSquare + forwardDirection;

            if (board[doubleTarget] == EMPTY) moves.push_back({from, doubleTarget});
        }
    }

    // Check pawn captures (including en passant)
    for (int direction : captureDirections) {
        targetSquare = from + direction;
        targetPiece = board[targetSquare];
        if ((targetPiece != EMPTY && targetPiece != OFFBOARD && !isSameColour(colour, targetPiece)) ||
            (targetSquare == enPassantSquare && enPassantSquare != -1)) {
            if (isPromotionRank(colour, targetRank)) {
                addPawnPromotionMoves(board[from], from, targetSquare, moves);
            } else {
                moves.push_back({from, targetSquare});
            }
        }
    }
}

void addPawnPromotionMoves(Piece pawn, int from, int targetSquare, vector<Move>& moves) {
    for (int i = 1; i <= 4; i++) {
        moves.push_back({from, targetSquare, pawn + i});
    }
}

void addKingMoves(const Position& pos, int from, vector<Move>& moves) {
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    int n = numDirections[KING];
    for (int i = 0; i < n; i++) {
        int targetSquare = from + moveDirections[KING][i];
        int targetPiece = board[targetSquare];

        if (targetPiece == OFFBOARD) continue;
        if (targetPiece == EMPTY || !isSameColour(colour, targetPiece)) moves.push_back({from, targetSquare});
    }

    for (int i = 0; i < 2; i++) {
        if (pos.canCastle[colour * 2 + i]) {
            int numSquares = (i == KINGSIDE) ? 2 : 3;
            bool valid = true;
            for (int j = 1; j <= numSquares; j++) {
                int targetSquare = from + j * castlingDirections[i];
                int targetPiece = board[targetSquare];

                if (targetPiece != EMPTY) {
                    valid = false;
                    break;
                }
            }
            if (valid) moves.push_back({from, from + castlingDirections[i] * 2});
        }
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
            addPawnMoves(pos, i, moves);
        } else if (pieceType == KING) {
            addKingMoves(pos, i, moves);
        }
        // etc...
    }

    return moves;
}