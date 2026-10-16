#include <cassert>
#include <bits/stdc++.h>
#include "../include/types.hpp"
#include "../include/movegen.hpp"
using namespace std;

void printMoves(vector<Move>& moves) {
    for (const Move& move : moves) {
        cout << move.from << " -> " << move.to << endl;
    }
}

bool hasMove(vector<Move>& moves, int from, int to) {
    for (const Move& move : moves) {
        if (move.from == from && move.to == to) return true;
    }
    return false;
}

void testOpenRook() {
    Position pos;

    pos.pieces[D4] = wR;

    vector<Move> moves = generateMoves(pos);

    assert(moves.size() == 14);
    assert(hasMove(moves, D4, D5));
    assert(hasMove(moves, D4, D8));
    assert(hasMove(moves, D4, D1));
    assert(hasMove(moves, D4, A4));
    assert(hasMove(moves, D4, H4));

    cout << "testOpenRook passed!\n";
}

void testBlockedRook() {
    Position pos;

    pos.pieces[G6] = wR;
    pos.pieces[G2] = wP;
    pos.pieces[D6] = wN;

    vector<Move> moves = generateMoves(pos);

    assert(hasMove(moves, G6, G8));
    assert(!hasMove(moves, G6, G2));
    assert(!hasMove(moves, G6, G1));
    assert(hasMove(moves, G6, H6));
    assert(!hasMove(moves, G6, D6));
    assert(!hasMove(moves, G6, C6));

    cout << "testBlockedRook passed!\n";
}

void testOpenBishop() {
    Position pos;

    pos.pieces[F5] = wB;

    vector<Move> moves = generateMoves(pos);

    assert(moves.size() == 11);
    assert(hasMove(moves, F5, C8));
    assert(hasMove(moves, F5, B1));
    assert(hasMove(moves, F5, H3));
    assert(hasMove(moves, F5, H7));

    cout << "testOpenBishop passed!\n";
}

void testBlockedBishop() {
    Position pos;

    pos.pieces[F5] = wB;
    pos.pieces[D7] = wP;
    pos.pieces[B1] = bN;

    vector<Move> moves = generateMoves(pos);

    assert(!hasMove(moves, F5, C8));
    assert(hasMove(moves, F5, B1));
    assert(hasMove(moves, F5, H3));
    assert(hasMove(moves, F5, H7));

    cout << "testOpenBishop passed!\n";
}

int main() {
    testOpenRook();
    testBlockedRook();
    testOpenBishop();
    testBlockedBishop();

    cout << "All tests passed!\n";
    return 0;
}