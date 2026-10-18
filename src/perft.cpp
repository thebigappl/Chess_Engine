#include "types.hpp"
#include "movegen.hpp"
#include "attacks.hpp"
#include "move.hpp"

U64 perft(Position& pos, int depth) {
    if (depth == 0) return 1;

    U64 nodes = 0;
    auto moves = generateMoves(pos);
    for (const Move& move : moves) {
        int colour = pos.sideToMove;
        UndoInfo undo = makeMove(pos, move);
        if (!isInCheck(pos, colour)) {
            nodes += perft(pos, depth - 1);
        }
        unmakeMove(pos, move, undo);
    }
    
    return nodes;
}