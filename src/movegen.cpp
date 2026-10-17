#include "../include/types.hpp"
#include "../include/movegen.hpp"
#include <vector>
#include <array>
using namespace std;

void addSlidingMoves(const Position& pos, int from, int pieceType, vector<Move>& moves);
int getPieceType(Piece piece);
bool isSameColour(int colour, Piece targetPiece);
void addKnightMoves(const Position& pos, int from, vector<Move>& moves);
void addPawnMoves(const Position& pos, int from, vector<Move>& moves);
void addPawnPromotionMoves(Piece pawn, int from, int targetSquare, vector<Move>& moves);
int getRank(int square);
int getFile(int square);
bool isStartingPosition(int colour, int rank);
bool isPromotionRank(int colour, int rank);

bool isSlidingPiece[6] = {false, false, true, true, true, false};
int numDirections[6] = {0, 8, 4, 4, 8, 8};
int moveDirections[6][8] = {
    {   0,   0,  0,  0, 0,  0,  0,  0 },
    { -21, -19,-12, -8, 8, 12, 19, 21 }, // Knight
    { -11,  -9,  9, 11, 0,  0,  0,  0 }, // Bishop
    { -10,  -1,  1, 10, 0,  0,  0,  0 }, // Rook
    { -11, -10, -9, -1, 1,  9, 10, 11 }, // Queen
    { -11, -10, -9, -1, 1,  9, 10, 11 }  // King
};
int pawnDirections[2] = {1, -1};
int captureDirections[2] = {11, 9};
int castlingDirections[2] = {1, -1};
int originalRookPositions[4] = {H1, A1, H8, A8};

// Assumes `from` contains a sliding piece of the side to move.
void addSlidingMoves(const Position& pos, int from, int pieceType, vector<Move>& moves) {
    // Sliding move generation only
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    for (int direction : moveDirections[pieceType]) {
        for (int i = 1; i < 8; i++) {
            int targetSquare = from + i * direction;
            int targetPiece = board[targetSquare];

            if (targetPiece == OFFBOARD) break;

            if (targetPiece == EMPTY) {
                moves.push_back({from, targetSquare});
                continue;
            }
            if (isSameColour(colour, targetPiece)) {
                break;
            } else {
                moves.push_back({from, targetSquare});
                break;
            }
        }
    }
}

// Gets the general pieceType (PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING or NONE)
int getPieceType(Piece piece) {
    if (piece == EMPTY || piece == OFFBOARD) return NONE;
    return piece % 6;
}

// Checks if the targetPiece is of the same colour as the side to move
bool isSameColour(int colour, Piece targetPiece) {
    if (colour == WHITE && (targetPiece >= wP && targetPiece <= wK)) return true;
    if (colour == BLACK && (targetPiece >= bP && targetPiece <= bK)) return true;
    return false;
}

int getRank(int square) {
    return (square - A1) / 10;
}

int getFile(int square) {
    return (square - A1) % 10;
}

bool isStartingPosition(int colour, int rank) {
    return ((colour == WHITE && rank == RANK_2) ||
            (colour == BLACK && rank == RANK_7));
}

bool isPromotionRank(int colour, int rank) {
    return ((colour == WHITE && rank == RANK_8) || 
            (colour == BLACK && rank == RANK_1));
}

// Helper for checking if a move direction is diagonal
bool isDiagonalDirection(int direction) {
    return (direction == -9 || direction == 9 ||
            direction == -11 || direction == 11);
}

void addKnightMoves(const Position& pos, int from, vector<Move>& moves) {
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    for (int direction : moveDirections[KNIGHT]) {
        int targetSquare = from + direction;
        int targetPiece = board[targetSquare];

        if (targetPiece == OFFBOARD) continue;
        if (targetPiece == EMPTY || !isSameColour(colour, targetPiece)) moves.push_back({from, targetSquare});
    }
}

void addPawnMoves(const Position& pos, int from, vector<Move>& moves) {
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    int enPassantSquare = pos.enPassantSquare;
    int fromRank = getRank(from);
    int pawnDirection = pawnDirections[colour];
    int forwardDirection = pawnDirection * 10;

    // First check normal pawn movement
    int targetSquare = from + forwardDirection;
    int targetRank = getRank(targetSquare);
    int targetPiece = board[targetSquare];
    if (targetPiece == EMPTY) {
        // To promote or not
        if (isPromotionRank(colour, targetRank)) {
            addPawnPromotionMoves(board[from], from, targetSquare, moves);
        } else {
            moves.push_back({from, targetSquare});
        }

        // Pawn double push
        if (isStartingPosition(colour, fromRank)) {
            int doubleTarget = targetSquare + forwardDirection;

            if (board[doubleTarget] == EMPTY) moves.push_back({from, doubleTarget});
        }
    }

    // Check pawn captures (including en passant)
    for (int direction : captureDirections) {
        targetSquare = from + direction;
        targetPiece = board[targetSquare];
        if ((targetPiece != EMPTY && targetPiece != OFFBOARD && !isSameColour(colour, targetPiece)) ||
            (targetSquare == enPassantSquare && enPassantSquare != -1)) {
            if (isPromotionRank(colour, targetRank)) {
                addPawnPromotionMoves(board[from], from, targetSquare, moves);
            } else {
                moves.push_back({from, targetSquare});
            }
        }
    }
}

// This helper function assumes that `targetSquare` lies on the promotion rank
void addPawnPromotionMoves(Piece pawn, int from, int targetSquare, vector<Move>& moves) {
    for (int i = 1; i <= 4; i++) {
        moves.push_back({from, targetSquare, pawn + i});
    }
}

void addKingMoves(const Position& pos, int from, vector<Move>& moves) {
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;
    for (int direction : moveDirections[KING]) {
        int targetSquare = from + direction;
        int targetPiece = board[targetSquare];

        if (targetPiece == OFFBOARD) continue;
        if (targetPiece == EMPTY || !isSameColour(colour, targetPiece)) moves.push_back({from, targetSquare});
    }

    for (int i = 0; i < 2; i++) {
        if (pos.canCastle[colour * 2 + i]) {
            int numSquares = (i == KINGSIDE) ? 2 : 3;
            bool valid = true;
            for (int j = 1; j <= numSquares; j++) {
                int targetSquare = from + j * castlingDirections[i];
                int targetPiece = board[targetSquare];

                if (targetPiece != EMPTY) {
                    valid = false;
                    break;
                }
            }
            if (valid) moves.push_back({from, from + castlingDirections[i] * 2});
        }
    }
}

//
// Functions for checking basic position validation (king safety through isSquareAttacked)
//
bool isPawnAttack(const Position& pos, int square, int attackingColour) {
    int attackDirection = pawnDirections[attackingColour];
    const Piece *board = pos.pieces;
    for (int direction : captureDirections) {
        int targetSquare = square - attackDirection * direction;
        int targetPiece = board[targetSquare];
        if (targetPiece == OFFBOARD || targetPiece == EMPTY) continue;
        if (getPieceType(targetPiece) == PAWN && 
            isSameColour(attackingColour, targetPiece)) return true;
    }
    return false;
}

bool isKnightAttack(const Position& pos, int square, int attackingColour) {
    const Piece *board = pos.pieces;
    for (int direction : moveDirections[KNIGHT]) {
        int targetSquare = square + direction;
        int targetPiece = board[targetSquare];
        if (targetPiece == OFFBOARD || targetPiece == EMPTY) continue;
        if (getPieceType(targetPiece) == KNIGHT &&
            isSameColour(attackingColour, targetPiece)) return true;
    }
    return false;
}

bool isSlidingAttack(const Position& pos, int square, int attackingColour) {
    const Piece *board = pos.pieces;
    for (int direction : moveDirections[QUEEN]) {
        for (int i = 1; i < 8; i++) {
            int targetSquare = square + i * direction;
            int targetPiece = board[targetSquare];

            if (targetPiece == OFFBOARD) break;

            if (targetPiece == EMPTY) continue;

            if (!isSameColour(attackingColour, targetPiece)) break;

            int pieceType = getPieceType(targetPiece);
            bool diagonal = isDiagonalDirection(direction);

            if (diagonal) {
                if (pieceType == BISHOP || pieceType == QUEEN) return true;
            } else {
                if (pieceType == ROOK || pieceType == QUEEN) return true;
            }
            break;
        }
    }
    return false;
}

bool isKingAttack(const Position& pos, int square, int attackingColour) {
    const Piece *board = pos.pieces;
    for (int direction : moveDirections[KING]) {
        int targetSquare = square + direction;
        int targetPiece = board[targetSquare];
        if (targetPiece == OFFBOARD || targetPiece == EMPTY) continue;
        if (getPieceType(targetPiece) == KING &&
            isSameColour(attackingColour, targetPiece)) return true;
    }
    return false;
}

bool isSquareAttacked(const Position& pos, int square, int attackingColour) {
    if (isPawnAttack(pos, square, attackingColour)) return true;
    if (isKnightAttack(pos, square, attackingColour)) return true;
    if (isKingAttack(pos, square, attackingColour)) return true;
    if (isSlidingAttack(pos, square, attackingColour)) return true;
    return false;
}

// This function assumes that `move` is a king move
bool isCastle(Move move) {
    int dist = move.to - move.from;
    if (dist == 2 || dist == -2) return true;
    return false;
}

UndoInfo makeMove(Position& pos, Move move) {
    Piece *board = pos.pieces;

    // Store previous state
    UndoInfo undo;
    undo.capturedPiece = board[move.to];
    undo.captureSquare = move.to;
    undo.previousKingSquares = pos.kingSquares;
    undo.previousCanCastle = pos.canCastle;
    undo.previousEnPassantSquare = pos.enPassantSquare;
    undo.previousHalfMoveClock = pos.halfMoveClock;


    // Modify current position state
    Piece movingPieceType = getPieceType(board[move.from]);
    board[move.to] = board[move.from];
    board[move.from] = EMPTY;

    // Check if the king moved
    if (movingPieceType == KING) {
        pos.kingSquares[pos.sideToMove] = move.to;
        // Castling
        if (isCastle(move)) {
            Square rookSquare;
            Square newRookSquare;
            if (move.to > move.from) {
                rookSquare = move.from + 3;
                newRookSquare = move.to - 1;
            } else {
                rookSquare = move.from - 4;
                newRookSquare = move.to + 1;
            }
            board[newRookSquare] = board[rookSquare];
            board[rookSquare] = EMPTY;
        }
        // Remove colour's right to castle on both sides
        for (int i = 0; i < 2; i++) {
            pos.canCastle[pos.sideToMove * 2 + i] = false;
        }
    }

    // Check if a rook moved
    if (movingPieceType == ROOK) {
        for (int i = 0; i < 4; i++) {
            if (move.from == originalRookPositions[i]) pos.canCastle[i] = false;
        }
    }

    // Check if a rook was captured
    if (getPieceType(undo.capturedPiece) == ROOK) {
        for (int i = 0; i < 4; i++) {
            if (move.to == originalRookPositions[i]) pos.canCastle[i] = false;
        }
    }

    // Check if move is irreversible (by the rules of the game)
    if (movingPieceType == PAWN || undo.capturedPiece != EMPTY) pos.halfMoveClock = 0;

    // Check promotion
    if (movingPieceType == PAWN &&
        isPromotionRank(pos.sideToMove, getRank(move.to))) board[move.to] = move.promotionPiece;

    // Check en passant
    if (movingPieceType == PAWN &&
        move.to == pos.enPassantSquare &&
        isDiagonalDirection(move.to - move.from)) {
        int captureSquare = move.to - pawnDirections[pos.sideToMove] * 10;
        undo.capturedPiece = board[captureSquare];
        undo.captureSquare = captureSquare;
        board[captureSquare] = EMPTY;
    }

    // Check double pawn push
    if (movingPieceType == PAWN &&
        (move.to - move.from) * pawnDirections[pos.sideToMove] == 20) {
        pos.enPassantSquare = (move.to + move.from) / 2;
    } else {
        pos.enPassantSquare = -1;
    }

    pos.sideToMove = (pos.sideToMove + 1) % 2;
    pos.halfMoves++;

    return undo;
}

void unmakeMove(Position& pos, Move move, const UndoInfo& undo) {
    Piece *board = pos.pieces;

    // Move the moving piece back to its original square
    board[move.from] = board[move.to];

    // If the move was a pawn promotion, revert the pawn back
    if (move.promotionPiece != EMPTY) board[move.from] -= getPieceType(board[move.from]);

    // Place the captured piece back on the capture square
    board[undo.captureSquare] = undo.capturedPiece;

    // If last move was a castle, also move the rook back
    if (isCastle(move)) {
        Square rookSquare;
        Square newRookSquare;
        if (move.to > move.from) {
            rookSquare = move.from - 1;
            newRookSquare = move.to + 3;
        } else {
            rookSquare = move.from + 1;
            newRookSquare = move.to - 4;
        }
        board[newRookSquare] = board[rookSquare];
        board[rookSquare] = EMPTY;
    }

    // Simple reverts
    pos.kingSquares = undo.previousKingSquares;
    pos.canCastle = undo.previousCanCastle;
    pos.enPassantSquare = undo.previousEnPassantSquare;
    pos.halfMoveClock = undo.previousHalfMoveClock;
    pos.sideToMove = (pos.sideToMove + 1) % 2;
    pos.halfMoves--;
}


// Focusing on only pseudo-legal moves currently
// This function will only be for generating pseudo-legal moves as well
// A new function `generateLegalMoves` will be used for filtering out illegal pseudo-legal moves
vector<Move> generateMoves(const Position& pos) {
    vector<Move> moves;
    const Piece *board = pos.pieces;
    int colour = pos.sideToMove;

    for (int i = 0; i < NUM_SQUARES; i++) {
        Piece piece = board[i];
        if (piece == OFFBOARD || piece == EMPTY) continue;
        if (!isSameColour(colour, piece)) continue;

        int pieceType = getPieceType(piece);
        if (isSlidingPiece[pieceType]) {
            addSlidingMoves(pos, i, pieceType, moves);
        } else if (pieceType == KNIGHT) {
            addKnightMoves(pos, i, moves);
        } else if (pieceType == PAWN) {
            addPawnMoves(pos, i, moves);
        } else if (pieceType == KING) {
            addKingMoves(pos, i, moves);
        }
        // etc...
    }

    return moves;
}