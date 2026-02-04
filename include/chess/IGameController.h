#pragma once

#include <string>
#include <vector>

#include "chess/Board.h"
#include "chess/Color.h"
#include "chess/GameStatus.h"
#include "chess/Position.h"

class IGameController {
 public:
  virtual ~IGameController() = default;

  virtual bool makeMove(const std::string& from, const std::string& to) = 0;
  virtual GameStatus status() const = 0;
  virtual Color turn() const = 0;
  virtual const Board& board() const = 0;
  virtual std::vector<Position> legalMovesFrom(const std::string& square) const = 0;
};
