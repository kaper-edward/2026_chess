#pragma once

#include "chess/Board.h"
#include "chess/RenderState.h"

class IBoardRenderer {
 public:
  virtual ~IBoardRenderer() = default;

  virtual void render(const Board& board, const RenderState& state) = 0;
};
