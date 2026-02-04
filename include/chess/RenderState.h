#pragma once

#include <optional>
#include <vector>

#include "chess/Position.h"

struct RenderState {
  std::optional<Position> selected;
  std::vector<Position> legalMoves;
};
