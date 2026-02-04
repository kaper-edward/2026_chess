#pragma once

#include <string>
#include <optional>

#include "chess/Position.h"

class CoordinateParser {
 public:
  static std::optional<Position> Parse(const std::string& input);
};
