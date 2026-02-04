#pragma once

#include <vector>
#include "chess/Board.h"
#include "chess/Position.h"

class MoveRules {
 public:
  static std::vector<Position> legalMoves(const Board& board, Position from);
};
