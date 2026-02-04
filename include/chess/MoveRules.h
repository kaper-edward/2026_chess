#pragma once

#include <vector>
#include "chess/IMoveRules.h"

class MoveRules final : public IMoveRules {
 public:
  // Returns pseudo-legal moves (ignores check).
  std::vector<Position> pseudoLegalMoves(const Board& board, Position from) const override;
};
