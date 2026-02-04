#pragma once

#include "chess/InputEvent.h"

class IInputProvider {
 public:
  virtual ~IInputProvider() = default;

  // Implementations may block (e.g., ncurses) or return immediately when no input
  // is available (e.g., SFML, returning InputEvent::Type::None).
  virtual InputEvent poll() = 0;
};
