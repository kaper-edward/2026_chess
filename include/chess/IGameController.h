#pragma once

#include <string>

#include "chess/Color.h"
#include "chess/GameStatus.h"

class IGameController {
 public:
  virtual ~IGameController() = default;

  virtual bool makeMove(const std::string& from, const std::string& to) = 0;
  virtual GameStatus status() const = 0;
  virtual Color turn() const = 0;
};
