#pragma once

#include "types.hpp"

UndoInfo makeMove(Position& pos, Move move);
void unmakeMove(Position& pos, Move move, const UndoInfo& undo);
