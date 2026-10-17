#include <algorithm>
#include <cassert>
#include <iostream>
#include "types.hpp"
#include "attacks.hpp"
#include "position.hpp"
#include "move.hpp"
using namespace std;

void testPawnAttacks() {
    Position pos;
    initialiseStartingPosition(pos);

    makeMove(pos, {E2, E4});
    assert(isSquareAttacked(pos, D5, WHITE) == true);
    assert(isSquareAttacked(pos, F5, WHITE) == true);

    cout << "testPawnAttacks passed!\n";
}

int main() {
    testPawnAttacks();

    cout << "All tests passed!\n";

    return 0;
}