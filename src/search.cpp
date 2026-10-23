#include "types.hpp"
#include "attacks.hpp"
#include "constants.hpp"
#include "evaluation.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int movePriorityScore(const Position& pos, const Move& move) {
    if (move.promotionPiece != EMPTY) return 3;
    if ((pos.pieces[move.from] == wP || pos.pieces[move.from] == bP) &&
        move.to == pos.enPassantSquare) return 2;
    if (isCapture(pos, move)) return 2;
    return 1;
}

bool priorityComparisonRule(const ScoredMove& a, const ScoredMove& b) {
    return a.priorityScore > b.priorityScore;
}

bool isQuiet(const Position& pos, const Move& move) {
    if (isCapture(pos, move)) return false;
    if (move.promotionPiece != EMPTY) return false;
    if ((pos.pieces[move.from] == wP || pos.pieces[move.from] == bP) &&
        move.to == pos.enPassantSquare) return false;
    return true;
}

// This function does not detect stalemates
int quiescenceSearch(Position& pos, int ply, U64& nodes, int alpha, int beta, int plyExplored) {
    nodes++;
    int standPatScore = evaluatePosition(pos);
    int colour = pos.sideToMove;
    bool inCheck = isInCheck(pos, colour);

    // Prevent Q-search from exploring too deep into tactical sequences
    if (!inCheck && plyExplored >= MAX_Q_DEPTH) return standPatScore;
    if (!inCheck && standPatScore >= beta) return standPatScore;
    if (!inCheck && standPatScore > alpha) alpha = standPatScore;

    // generate moves only once
    MoveList moveList = generateMoves(pos);

    // Move ordering
    // Create a priority list and sort the moves based on how good they might be
    ScoredMove priorityMoveList[256];
    int n = moveList.count;
    for (int i = 0; i < n; i++) {
        priorityMoveList[i].move = moveList.moves[i];
        priorityMoveList[i].priorityScore = movePriorityScore(pos, priorityMoveList[i].move);
    }
    sort(priorityMoveList, priorityMoveList + n, priorityComparisonRule);

    int bestScore = inCheck ? -INFINITY_SCORE : standPatScore;
    bool isLegal = false;
    for (int i = 0; i < n; i++) {
        const Move& move = priorityMoveList[i].move;
        // Skip quiet moves
        // Skip if the position isn't a check
        bool quiet = isQuiet(pos, move);
        if (!inCheck && quiet) {
            continue;
        }
        
        UndoInfo undo = makeMove(pos, move);
        // Negate the result because the next call to search
        // is from the perspective of the other player
        if (!isInCheck(pos, colour)) {
            int score = -quiescenceSearch(pos, ply + 1, nodes, -beta, -alpha, plyExplored + 1);
            bestScore = max(bestScore, score);
            alpha = max(alpha, score);
            isLegal = true;
        }

        unmakeMove(pos, move, undo);

        if (alpha >= beta) break;
    }
    
    if (!isLegal) return inCheck ? -(MATE_SCORE - ply) : bestScore;
    return bestScore;
}

// Recursive search function for finding the best move
int search(Position& pos, int depth, int ply, U64& nodes, int alpha, int beta) {
    nodes++;
    int colour = pos.sideToMove;
    bool inCheck = isInCheck(pos, colour);

    // generate moves only once
    MoveList moveList = generateMoves(pos);

    // Move ordering
    // Create a priority list and sort the moves based on how good they might be
    ScoredMove priorityMoveList[256];
    int n = moveList.count;
    for (int i = 0; i < n; i++) {
        priorityMoveList[i].move = moveList.moves[i];
        priorityMoveList[i].priorityScore = movePriorityScore(pos, priorityMoveList[i].move);
    }
    sort(priorityMoveList, priorityMoveList + n, priorityComparisonRule);

    int bestScore = -INFINITY_SCORE;
    bool isLegal = false;
    for (int i = 0; i < n; i++) {
        const Move& move = priorityMoveList[i].move;
        UndoInfo undo = makeMove(pos, move);
        // Negate the result because the next call to search
        // is from the perspective of the other player
        if (!isInCheck(pos, colour)) {
            if (depth > 0) {
                int score = -search(pos, depth - 1, ply + 1, nodes, -beta, -alpha);
                bestScore = max(bestScore, score);
                alpha = max(alpha, score);
            }
            isLegal = true;
        }

        unmakeMove(pos, move, undo);

        if (alpha >= beta || (isLegal && depth == 0)) break;
    }
    
    if (!isLegal) return inCheck ? -(MATE_SCORE - ply) : 0;

    if (depth == 0) {
        int score = quiescenceSearch(pos, ply, nodes, alpha, beta, 0);
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
    Move prevBestMove{-1, -1, EMPTY};
    SearchResult res;

    // Move ordering setup
    ScoredMove priorityMoveList[256];
    int n = moveList.count;

    // Iterative deepening loop
    for (int i = 1; i <= depth; i++) {
        // Reset best move
        bestMove.from = -1;
        bestMove.to = -1;
        bestMove.promotionPiece = EMPTY;
        bestScore = -INFINITY_SCORE;

        // Reorder move list based on priority factors again
        for (int j = 0; j < n; j++) {
            priorityMoveList[j].move = moveList.moves[j];
            priorityMoveList[j].priorityScore = movePriorityScore(pos, priorityMoveList[j].move);
            if (priorityMoveList[j].move == prevBestMove) {
                priorityMoveList[j].priorityScore += 100;
            }
        }
        sort(priorityMoveList, priorityMoveList + n, priorityComparisonRule);

        for (int currMove = 0; currMove < n; currMove++) {
            const Move& move = priorityMoveList[currMove].move;
            UndoInfo undo = makeMove(pos, move);
    
            if (!isInCheck(pos, colour)) {
                int score = -search(pos, i - 1, 1, res.nodes, -INFINITY_SCORE, INFINITY_SCORE);
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                }
            }
    
            unmakeMove(pos, move, undo);
        }
        prevBestMove = bestMove;
    }

    res.bestMove = bestMove;
    res.score = bestScore;
    return res;
}