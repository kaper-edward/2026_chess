#pragma once

#include <vector>
#include "chess/Board.h"
#include "chess/Position.h"

class MoveRules {
 public:
  // Returns pseudo-legal moves (ignores check).
  static std::vector<Position> pseudoLegalMoves(const Board& board, Position from);
};
