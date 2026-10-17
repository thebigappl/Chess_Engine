#include "types.hpp"

// Assumes that `pos` is an empty board
// This function is not well designed lol
void initialiseStartingPosition(Position& pos) {
    pos.kingSquares[WHITE] = E1;
    pos.kingSquares[BLACK] = E8;
    pos.sideToMove = WHITE;
    pos.canCastle.fill(true);
    pos.enPassantSquare = -1;
    pos.halfMoves = 0;
    pos.halfMoveClock = 0;

    Piece *board = pos.pieces;
    board[A1] = wR;
    board[B1] = wN;
    board[C1] = wB;
    board[D1] = wQ;
    board[E1] = wK;
    board[F1] = wB;
    board[G1] = wN;
    board[H1] = wR;
    for (int i = A2; i <= H2; i++) {
        board[i] = wP;
    }
    board[A8] = bR;
    board[B8] = bN;
    board[C8] = bB;
    board[D8] = bQ;
    board[E8] = bK;
    board[F8] = bB;
    board[G8] = bN;
    board[H8] = bR;
    for (int i = A7; i <= H7; i++) {
        board[i] = bP;
    }
}