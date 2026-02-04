#pragma once

#include <string>
#include <optional>

#include "chess/Position.h"

class CoordinateParser {
 public:
  static std::optional<Position> parse(const std::string& input);
};
