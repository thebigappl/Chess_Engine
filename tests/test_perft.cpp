#include <cassert>
#include <iostream>
#include "types.hpp"
#include "movegen.hpp"
#include "position.hpp"
#include "move.hpp"
#include "attacks.hpp"
#include "perft.hpp"
using namespace std;

void testPerft() {
    Position pos;
    initialiseStartingPosition(pos);

    assert(perft(pos, 1) == 20);
    assert(perft(pos, 2) == 400);
    assert(perft(pos, 3) == 8902);
    assert(perft(pos, 4) == 197281);
    assert(perft(pos, 5) == 4865609);

    // This takes way too long rn
    // assert(perft(pos, 6) == 119060324);

    cout << "testPerft passed!\n";
}

int main() {
    testPerft();

    cout << "All tests passed!\n";

    return 0;
}