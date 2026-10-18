#include "types.hpp"
#include "pieces.hpp"
#include "constants.hpp"

// This function evaluates who is winning between black and white given the current board position
// It currently only considers material advantages and answers from white's perspective
int evaluatePosition(const Position& pos) {
    int materialAdvantage = 0;
    const Piece *board = pos.pieces;
    for (int i = 0; i < NUM_SQUARES; i++) {
        Piece targetPiece = board[i];
        if (targetPiece == OFFBOARD || targetPiece == EMPTY) continue;

        int pieceType = getPieceType(targetPiece);
        int pieceValue = pieceValues[pieceType];

        if (isSameColour(WHITE, targetPiece)) {
            materialAdvantage += pieceValue;
        } else {
            materialAdvantage -= pieceValue;
        }
    }
    
    return materialAdvantage;
}
