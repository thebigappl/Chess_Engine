#ifndef TYPES_HPP
#define TYPES_HPP

#include <array>
using namespace std;
using U64 = unsigned long long;
using Square = int;
using Piece = int;

inline constexpr char NAME[] = "Engine";
inline constexpr int NUM_SQUARES = 120;

enum {
    wP, wN, wB, wR, wQ, wK,
    bP, bN, bB, bR, bQ, bK,
    EMPTY, OFFBOARD
};

enum {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE
};

enum {
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE
};

enum {
    WHITE, BLACK, BOTH
};

enum {
    KINGSIDE, QUEENSIDE
};

enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

struct Position {
    Piece pieces[NUM_SQUARES];

    array<Square, 2> kingSquares = {E1, E8};

    int sideToMove = WHITE;
    array<bool, 4> canCastle = {true, true, true, true}; // wK, wQ, bK, bQ
    Square enPassantSquare = -1;
    int halfMoves = 0;
    int halfMoveClock = 0;

    // Initialise a board containing 64 EMPTY squares and the remaining 56 are OFFBOARD
    Position() {
        for (int i = 0; i < NUM_SQUARES; i++) {
            pieces[i] = OFFBOARD;
        }

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                pieces[(i + 2) * 10 + (j + 1)] = EMPTY;
            }
        }
    }
};

struct Move {
    Square from;
    Square to;
    int promotionPiece = EMPTY;
};

struct UndoInfo {
    Piece capturedPiece;
    Square captureSquare;

    array<Square, 2> previousKingSquares;

    Square previousEnPassantSquare;
    array<bool, 4> previousCanCastle;

    int previousHalfMoveClock;
};

#endif