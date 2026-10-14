#include <bits/stdc++.h>
#include <types.hpp>

int main() {
    // Initialise new game
    Game board = {
        {
            OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
            OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
            OFFBOARD, wR, wN, wB, wQ, wK, wB, wN, wR, OFFBOARD,
            OFFBOARD, wP, wP, wP, wP, wP, wP, wP, wP, OFFBOARD,
            OFFBOARD, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, OFFBOARD,
            OFFBOARD, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, OFFBOARD,
            OFFBOARD, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, OFFBOARD,
            OFFBOARD, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, OFFBOARD,
            OFFBOARD, bP, bP, bP, bP, bP, bP, bP, bP, OFFBOARD,
            OFFBOARD, bR, bN, bB, bQ, bK, bB, bN, bR, OFFBOARD,
            OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
            OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
        },
        {E1, E8}, // kingSquares[2]
        WHITE, // sideToMove
        {true, true, true, true}, // castlingRights[4]
        -1, // enPassantSquare
        0, // halfMoves
        0 // reversibleMoves (consecutive moves without capture)
    };

    return 0;
}