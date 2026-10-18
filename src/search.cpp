#include "types.hpp"
#include "attacks.hpp"
#include "constants.hpp"
#include "evaluation.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include <climits>
#include <vector>
using namespace std;

// Recursive search function for finding the best move
int negamax(Position& pos, int depth) {
    if (isCheckmate(pos)) return -MATE_SCORE;
    if (isStalemate(pos)) return 0;
    if (depth == 0) {
        int score = evaluatePosition(pos);
        if (pos.sideToMove == BLACK) score = -score;
        return score;
    }

    int bestScore = INT_MIN;
    vector<Move> legalMoves = generateLegalMoves(pos);
    for (Move move : legalMoves) {
        UndoInfo undo = makeMove(pos, move);
        // Negate the result because the next call to negamax
        // is from the perspective of the other player
        int score = -negamax(pos, depth - 1);
        if (score > bestScore) bestScore = score;

        unmakeMove(pos, move, undo);
    }
    
    return bestScore;
}

// Assumes that current side to move has at least 1 legal move (not checkmate or stalemate)
Move findBestMove(Position& pos, int depth) {
    vector<Move> legalMoves = generateLegalMoves(pos);
    int bestScore = INT_MIN;
    Move bestMove;
    for (Move move : legalMoves) {
        UndoInfo undo = makeMove(pos, move);

        int score = -negamax(pos, depth - 1);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        unmakeMove(pos, move, undo);
    }

    return bestMove;
}