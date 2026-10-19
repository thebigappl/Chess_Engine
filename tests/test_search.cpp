#include <cassert>
#include <iostream>
#include "types.hpp"
#include "movegen.hpp"
#include "position.hpp"
#include "move.hpp"
#include "attacks.hpp"
#include "search.hpp"
using namespace std;

void testMateInOne() {
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
    
    U64 nodes = 0;
    SearchResult searchRes = findBestMove(pos, 2);

    assert(searchRes.bestMove.from == H5);
    assert(searchRes.bestMove.to == G6);

    cout << "testMateInOne passed!\n";
}

int main() {
    testMateInOne();

    cout << "All tests passed!\n";

    return 0;
}