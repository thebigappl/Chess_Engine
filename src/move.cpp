#include "types.hpp"
#include "movegen.hpp"
#include "pieces.hpp"
#include "constants.hpp"
#include "board.hpp"
#include "attacks.hpp"
#include <vector>
#include <array>
using namespace std;

// This function assumes that `move` is a king move
bool isCastle(Move move) {
    int dist = move.to - move.from;
    if (dist == 2 || dist == -2) return true;
    return false;
}

UndoInfo makeMove(Position& pos, Move move) {
    Piece *board = pos.pieces;

    // Store previous state
    UndoInfo undo;
    undo.capturedPiece = board[move.to];
    undo.captureSquare = move.to;
    undo.previousKingSquares = pos.kingSquares;
    undo.previousCanCastle = pos.canCastle;
    undo.previousEnPassantSquare = pos.enPassantSquare;
    undo.previousHalfMoveClock = pos.halfMoveClock;


    // Modify current position state
    Piece movingPieceType = getPieceType(board[move.from]);
    board[move.to] = board[move.from];
    board[move.from] = EMPTY;

    // Check if the king moved
    if (movingPieceType == KING) {
        pos.kingSquares[pos.sideToMove] = move.to;
        // Castling
        if (isCastle(move)) {
            Square rookSquare;
            Square newRookSquare;
            if (move.to > move.from) {
                rookSquare = move.from + 3;
                newRookSquare = move.to - 1;
            } else {
                rookSquare = move.from - 4;
                newRookSquare = move.to + 1;
            }
            board[newRookSquare] = board[rookSquare];
            board[rookSquare] = EMPTY;
        }
        // Remove colour's right to castle on both sides
        for (int i = 0; i < 2; i++) {
            pos.canCastle[pos.sideToMove * 2 + i] = false;
        }
    }

    // Check if a rook moved
    if (movingPieceType == ROOK) {
        for (int i = 0; i < 4; i++) {
            if (move.from == originalRookPositions[i]) pos.canCastle[i] = false;
        }
    }

    // Check if a rook was captured
    if (getPieceType(undo.capturedPiece) == ROOK) {
        for (int i = 0; i < 4; i++) {
            if (move.to == originalRookPositions[i]) pos.canCastle[i] = false;
        }
    }

    // Check if move is irreversible (by the rules of the game)
    if (movingPieceType == PAWN || undo.capturedPiece != EMPTY) pos.halfMoveClock = 0;

    // Check promotion
    if (movingPieceType == PAWN &&
        isPromotionRank(pos.sideToMove, getRank(move.to))) board[move.to] = move.promotionPiece;

    // Check en passant
    if (movingPieceType == PAWN &&
        move.to == pos.enPassantSquare &&
        isDiagonalDirection(move.to - move.from)) {
        int captureSquare = move.to - pawnDirections[pos.sideToMove] * 10;
        undo.capturedPiece = board[captureSquare];
        undo.captureSquare = captureSquare;
        board[captureSquare] = EMPTY;
    }

    // Check double pawn push
    if (movingPieceType == PAWN &&
        (move.to - move.from) * pawnDirections[pos.sideToMove] == 20) {
        pos.enPassantSquare = (move.to + move.from) / 2;
    } else {
        pos.enPassantSquare = -1;
    }

    pos.sideToMove = (pos.sideToMove + 1) % 2;
    pos.ply++;

    return undo;
}

void unmakeMove(Position& pos, Move move, const UndoInfo& undo) {
    Piece *board = pos.pieces;

    // Move the moving piece back to its original square
    board[move.from] = board[move.to];

    // If the move was a pawn promotion, revert the pawn back
    if (move.promotionPiece != EMPTY) board[move.from] -= getPieceType(board[move.from]);

    // Place the captured piece back on the capture square
    board[undo.captureSquare] = undo.capturedPiece;

    // If last move was a castle, also move the rook back
    if (isCastle(move)) {
        Square rookSquare;
        Square newRookSquare;
        if (move.to > move.from) {
            rookSquare = move.from - 1;
            newRookSquare = move.to + 3;
        } else {
            rookSquare = move.from + 1;
            newRookSquare = move.to - 4;
        }
        board[newRookSquare] = board[rookSquare];
        board[rookSquare] = EMPTY;
    }

    // Simple reverts
    pos.kingSquares = undo.previousKingSquares;
    pos.canCastle = undo.previousCanCastle;
    pos.enPassantSquare = undo.previousEnPassantSquare;
    pos.halfMoveClock = undo.previousHalfMoveClock;
    pos.sideToMove = (pos.sideToMove + 1) % 2;
    pos.ply--;
}