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
    
    // generate moves only once
    vector<Move> legalMoves = generateLegalMoves(pos);
    if (legalMoves.empty()) {
        // checkmate is horrible for side to move
        if (isInCheck(pos, pos.sideToMove)) return -(MATE_SCORE - ply);
        // stalemate
        return 0;
    }

    if (depth == 0) {
        int score = evaluatePosition(pos);
        return pos.sideToMove == BLACK ? -score : score;
    }

    int bestScore = -INFINITY_SCORE;
    for (Move move : legalMoves) {
        UndoInfo undo = makeMove(pos, move);
        // Negate the result because the next call to search
        // is from the perspective of the other player
        int score = -search(pos, depth - 1, ply + 1, nodes);
        if (score > bestScore) bestScore = score;

        unmakeMove(pos, move, undo);
    }
    
    return bestScore;
}

// Assumes that current side to move has at least 1 legal move (not checkmate or stalemate)
SearchResult findBestMove(Position& pos, int depth) {
    vector<Move> legalMoves = generateLegalMoves(pos);
    int bestScore = -INFINITY_SCORE;
    Move bestMove;
    SearchResult res;
    for (Move move : legalMoves) {
        UndoInfo undo = makeMove(pos, move);

        int score = -search(pos, depth - 1, 1, res.nodes);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        unmakeMove(pos, move, undo);
    }

    res.bestMove = bestMove;
    res.score = bestScore;
    return res;
}