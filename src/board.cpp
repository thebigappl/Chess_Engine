#include "types.hpp"

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