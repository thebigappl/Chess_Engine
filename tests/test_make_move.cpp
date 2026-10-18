#include <algorithm>
#include <cassert>
#include <iostream>
#include "types.hpp"
#include "move.hpp"
using namespace std;

void assertPositionEqual(const Position& a, const Position& b) {
    for (int i = 0; i < NUM_SQUARES; i++) {
        assert(a.pieces[i] == b.pieces[i]);
    }

    assert(a.canCastle == b.canCastle);
    assert(a.enPassantSquare == b.enPassantSquare);
    assert(a.halfMoveClock == b.halfMoveClock);
    assert(a.ply == b.ply);
    assert(a.kingSquares == b.kingSquares);
    assert(a.sideToMove == b.sideToMove);
}

void testMakeUnmake(Position& pos, Move move) {
    Position original = pos;
    
    UndoInfo undo = makeMove(pos, move);
    unmakeMove(pos, move, undo);

    assertPositionEqual(original, pos);
}

void testSimpleMoves() {
    Position pos;

    pos.pieces[E2] = wP;

    // Test basic funtionality of making a move and immediately unmaking
    testMakeUnmake(pos, {E2, E3});

    // Test double pawn push changes en passant square
    Position original = pos;
    UndoInfo undo = makeMove(pos, {E2, E4});
    assert(pos.enPassantSquare == E3);
    unmakeMove(pos, {E2, E4}, undo);
    assertPositionEqual(original, pos);

    // Test knight moves
    pos.pieces[G1] = wK;
    testMakeUnmake(pos, {G1, F3});
    
    // Test basic capture
    pos.pieces[D3] = bP;
    original = pos;
    undo = makeMove(pos, {E2, D3});
    assert(undo.capturedPiece == bP);
    unmakeMove(pos, {E2, D3}, undo);
    assertPositionEqual(original, pos);

    // Test basic king movement
    pos.pieces[A1] = wK;
    original = pos;
    undo = makeMove(pos, {A1, A2});
    assert(pos.kingSquares[WHITE] == A2);
    unmakeMove(pos, {A1, A2}, undo);
    assertPositionEqual(original, pos);

    cout << "testSimpleMoves passed!\n";
}

int main() {
    testSimpleMoves();

    cout << "All tests passed!\n";

    return 0;
}