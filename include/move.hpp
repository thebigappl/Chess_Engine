#pragma once

#include "types.hpp"

bool isCastle(Move move);
UndoInfo makeMove(Position& pos, Move move);
void unmakeMove(Position& pos, Move move, const UndoInfo& undo);
