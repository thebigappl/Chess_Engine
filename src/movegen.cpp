#include "types.hpp"
#include "movegen.hpp"
#include "pieces.hpp"
#include "constants.hpp"
#include "board.hpp"
#include "attacks.hpp"
#include <vector>
#include <array>
using namespace std;

// This helper function assumes that `targetSquare` lies on the promotion rank
void addPawnPromotionMoves(Piece pawn, int from, int targetSquare, vector<Move>& moves) {
    for (int i = 1; i <= 4; i++) {
        moves.push_back({from, targetSquare, pawn + i});
    }
}

// Assumes `from` contains a sliding piece of the side to move.
void addSlidingMoves(const Position& pos, int from, int pieceType, vector<Move>& moves) {
    // Sliding move generation only
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    for (int direction : moveDirections[pieceType]) {
        for (int i = 1; i < 8; i++) {
            int targetSquare = from + i * direction;
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

void addKnightMoves(const Position& pos, int from, vector<Move>& moves) {
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    for (int direction : moveDirections[KNIGHT]) {
        int targetSquare = from + direction;
        int targetPiece = board[targetSquare];

        if (targetPiece == OFFBOARD) continue;
        if (targetPiece == EMPTY || !isSameColour(colour, targetPiece)) moves.push_back({from, targetSquare});
    }
}

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
        // To promote or not
        if (isPromotionRank(colour, targetRank)) {
            addPawnPromotionMoves(board[from], from, targetSquare, moves);
        } else {
            moves.push_back({from, targetSquare});
        }

        // Pawn double push
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

void addKingMoves(const Position& pos, int from, vector<Move>& moves) {
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    for (int direction : moveDirections[KING]) {
        int targetSquare = from + direction;
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
// This function will only be for generating pseudo-legal moves as well
// A new function `generateLegalMoves` will be used for filtering out illegal pseudo-legal moves
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