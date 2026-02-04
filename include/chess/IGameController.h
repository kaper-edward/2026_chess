#pragma once

#include <string>

#include "chess/Color.h"
#include "chess/GameState.h"

class IGameController {
 public:
  virtual ~IGameController() = default;

  virtual bool MakeMove(const std::string& from, const std::string& to) = 0;
  virtual GameStatus Status() const = 0;
  virtual Color Turn() const = 0;
};
