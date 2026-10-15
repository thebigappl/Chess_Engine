#include <bits/stdc++.h>
#include "../include/types.hpp"
using namespace std;

int main() {
    // Initialise new game
    Position pos;
    // Print empty position
    for (int i = 0; i < NUM_SQUARES; i++) {
        if (i % 10 == 0) cout << endl;
        cout << pos.pieces[i] << " ";
    }
    cout << endl;

    return 0;
}