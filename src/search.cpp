#include "types.hpp"
#include "attacks.hpp"
#include "constants.hpp"
#include "evaluation.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include <vector>
#include <iostream>
using namespace std;

// Recursive search function for finding the best move
int search(Position& pos, int depth, int ply, U64& nodes) {
    nodes++;
    int colour = pos.sideToMove;
    bool inCheck = isInCheck(pos, colour);

    // generate moves only once
    MoveList moveList = generateMoves(pos);
    int bestScore = -INFINITY_SCORE;
    bool valid = false;
    for (const Move& move : moveList) {
        UndoInfo undo = makeMove(pos, move);
        // Negate the result because the next call to search
        // is from the perspective of the other player
        if (!isInCheck(pos, colour)) {
            if (depth > 0) {
                int score = -search(pos, depth - 1, ply + 1, nodes);
                if (score > bestScore) bestScore = score;
            }
            valid = true;
        }

        unmakeMove(pos, move, undo);

        if (valid && depth == 0) break;
    }
    
    if (!valid) return inCheck ? -(MATE_SCORE - ply) : 0;

    if (depth == 0) {
        int score = evaluatePosition(pos);
        return colour == BLACK ? -score : score;
    }

    return bestScore;
}

// Assumes that current side to move has at least 1 legal move (not checkmate or stalemate)
SearchResult findBestMove(Position& pos, int depth) {
    MoveList moveList = generateMoves(pos);
    int colour = pos.sideToMove;
    int bestScore = -INFINITY_SCORE;
    Move bestMove;
    SearchResult res;
    for (const Move& move : moveList) {
        UndoInfo undo = makeMove(pos, move);

        if (!isInCheck(pos, colour)) {
            int score = -search(pos, depth - 1, 1, res.nodes);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }

        unmakeMove(pos, move, undo);
    }

    res.bestMove = bestMove;
    res.score = bestScore;
    return res;
}