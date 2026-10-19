#include <cassert>
#include <iostream>
#include "types.hpp"
#include "movegen.hpp"
#include "position.hpp"
#include "move.hpp"
#include "attacks.hpp"
using namespace std;

void printMoves(const MoveList& moveList) {
    for (const Move& move : moveList) {
        cout << move.from << " -> " << move.to << endl;
    }
}

bool hasMove(MoveList& moveList, int from, int to) {
    for (const Move& move : moveList) {
        if (move.from == from && move.to == to) return true;
    }
    return false;
}

void testOpenRook() {
    Position pos;

    pos.pieces[D4] = wR;

    MoveList moveList = generateMoves(pos);

    assert(moveList.count == 14);
    assert(hasMove(moveList, D4, D5));
    assert(hasMove(moveList, D4, D8));
    assert(hasMove(moveList, D4, D1));
    assert(hasMove(moveList, D4, A4));
    assert(hasMove(moveList, D4, H4));

    cout << "testOpenRook passed!\n";
}

void testBlockedRook() {
    Position pos;

    pos.pieces[G6] = wR;
    pos.pieces[G2] = wP;
    pos.pieces[D6] = wN;

    MoveList moveList = generateMoves(pos);

    assert(hasMove(moveList, G6, G8));
    assert(!hasMove(moveList, G6, G2));
    assert(!hasMove(moveList, G6, G1));
    assert(hasMove(moveList, G6, H6));
    assert(!hasMove(moveList, G6, D6));
    assert(!hasMove(moveList, G6, C6));

    cout << "testBlockedRook passed!\n";
}

void testOpenBishop() {
    Position pos;

    pos.pieces[F5] = wB;

    MoveList moveList = generateMoves(pos);

    assert(moveList.count == 11);
    assert(hasMove(moveList, F5, C8));
    assert(hasMove(moveList, F5, B1));
    assert(hasMove(moveList, F5, H3));
    assert(hasMove(moveList, F5, H7));

    cout << "testOpenBishop passed!\n";
}

void testBlockedBishop() {
    Position pos;

    pos.pieces[F5] = wB;
    pos.pieces[D7] = wP;
    pos.pieces[B1] = bN;

    MoveList moveList = generateMoves(pos);

    assert(!hasMove(moveList, F5, C8));
    assert(hasMove(moveList, F5, B1));
    assert(hasMove(moveList, F5, H3));
    assert(hasMove(moveList, F5, H7));

    cout << "testOpenBishop passed!\n";
}

void testLegalMoves() {
    Position pos;
    initialiseStartingPosition(pos);

    // Let's play an actual chess opening

    makeMove(pos, {E2, E4});
    makeMove(pos, {H7, H6});
    makeMove(pos, {E4, E5});
    makeMove(pos, {F7, F5});
    makeMove(pos, {D1, H5});

    assert(generateLegalMoves(pos).count == 1);

    makeMove(pos, {G7, G6});
    makeMove(pos, {H5, G6});

    MoveList legalMoves = generateLegalMoves(pos);
    printMoves(legalMoves);

    assert(isCheckmate(pos) == true);

    cout << "testLegalMoves passed!\n";
}

int main() {
    testOpenRook();
    testBlockedRook();
    testOpenBishop();
    testBlockedBishop();
    testLegalMoves();

    cout << "All tests passed!\n";
    return 0;
}