#pragma once

#include "chess/ICheckDetector.h"

class CheckDetector final : public ICheckDetector {
 public:
  bool inCheck(const Board& board, Color color) const override;
};
