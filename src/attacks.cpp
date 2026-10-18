#include "pieces.hpp"
#include "constants.hpp"
#include "movegen.hpp"
#include <vector>

// Helper for checking if a move direction is diagonal
bool isDiagonalDirection(int direction) {
    return (direction == -9 || direction == 9 ||
            direction == -11 || direction == 11);
}

//
// Functions for checking basic position validation (king safety through isSquareAttacked)
//
bool isPawnAttack(const Position& pos, int square, int attackingColour) {
    int attackDirection = pawnDirections[attackingColour];
    const Piece *board = pos.pieces;
    for (int direction : captureDirections) {
        int targetSquare = square - attackDirection * direction;
        int targetPiece = board[targetSquare];
        if (targetPiece == OFFBOARD || targetPiece == EMPTY) continue;
        if (getPieceType(targetPiece) == PAWN && 
            isSameColour(attackingColour, targetPiece)) return true;
    }
    return false;
}

bool isKnightAttack(const Position& pos, int square, int attackingColour) {
    const Piece *board = pos.pieces;
    for (int direction : moveDirections[KNIGHT]) {
        int targetSquare = square + direction;
        int targetPiece = board[targetSquare];
        if (targetPiece == OFFBOARD || targetPiece == EMPTY) continue;
        if (getPieceType(targetPiece) == KNIGHT &&
            isSameColour(attackingColour, targetPiece)) return true;
    }
    return false;
}

bool isSlidingAttack(const Position& pos, int square, int attackingColour) {
    const Piece *board = pos.pieces;
    for (int direction : moveDirections[QUEEN]) {
        for (int i = 1; i < 8; i++) {
            int targetSquare = square + i * direction;
            int targetPiece = board[targetSquare];

            if (targetPiece == OFFBOARD) break;

            if (targetPiece == EMPTY) continue;

            if (!isSameColour(attackingColour, targetPiece)) break;

            int pieceType = getPieceType(targetPiece);
            bool diagonal = isDiagonalDirection(direction);

            if (diagonal) {
                if (pieceType == BISHOP || pieceType == QUEEN) return true;
            } else {
                if (pieceType == ROOK || pieceType == QUEEN) return true;
            }
            break;
        }
    }
    return false;
}

bool isKingAttack(const Position& pos, int square, int attackingColour) {
    const Piece *board = pos.pieces;
    for (int direction : moveDirections[KING]) {
        int targetSquare = square + direction;
        int targetPiece = board[targetSquare];
        if (targetPiece == OFFBOARD || targetPiece == EMPTY) continue;
        if (getPieceType(targetPiece) == KING &&
            isSameColour(attackingColour, targetPiece)) return true;
    }
    return false;
}

bool isSquareAttacked(const Position& pos, int square, int attackingColour) {
    if (isPawnAttack(pos, square, attackingColour)) return true;
    if (isKnightAttack(pos, square, attackingColour)) return true;
    if (isKingAttack(pos, square, attackingColour)) return true;
    if (isSlidingAttack(pos, square, attackingColour)) return true;
    return false;
}

bool isInCheck(const Position& pos, int colour) {
    return isSquareAttacked(pos, pos.kingSquares[colour], (colour + 1) % 2);
}

bool isCheckmate(const Position& pos) {
    return isInCheck(pos, pos.sideToMove) && generateLegalMoves(pos).empty();
}

bool isStalemate(const Position& pos) {
    return !isInCheck(pos, pos.sideToMove) && generateLegalMoves(pos).empty();
}

bool isFiftyMoveDraw(const Position& pos) {
    return pos.halfMoveClock >= 100;
}
