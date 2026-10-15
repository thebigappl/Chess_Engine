#include <cassert>
#include <bits/stdc++.h>
#include "../include/types.hpp"
#include "../include/movegen.hpp"
using namespace std;

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

    assert(moves.size() == 8);
    assert(hasMove(moves, G6, G8));
    assert(!hasMove(moves, G6, G2));
    assert(!hasMove(moves, G6, G1));
    assert(hasMove(moves, G6, H6));
    assert(!hasMove(moves, G6, D6));
    assert(!hasMove(moves, G6, C6));

    cout << "testBlockedRook passed!\n";
}

int main() {
    testOpenRook();
    testBlockedRook();

    cout << "All tests passed!\n";
    return 0;
}