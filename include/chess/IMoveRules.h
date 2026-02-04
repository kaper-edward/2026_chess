#pragma once

#include <vector>
#include "chess/Board.h"
#include "chess/Position.h"

class IMoveRules {
 public:
  virtual ~IMoveRules() = default;

  virtual std::vector<Position> pseudoLegalMoves(const Board& board, Position from) const = 0;
};
