#pragma once

#include "chess/Board.h"
#include "chess/Color.h"

class ICheckDetector {
 public:
  virtual ~ICheckDetector() = default;

  virtual bool inCheck(const Board& board, Color color) const = 0;
};
