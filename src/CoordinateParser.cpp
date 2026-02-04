#include "chess/CoordinateParser.h"

#include <cctype>

std::optional<Position> CoordinateParser::parse(const std::string& input) {
  if (input.size() != 2) {
    return std::nullopt;
  }

  char file = static_cast<char>(std::tolower(static_cast<unsigned char>(input[0])));
  char rank = input[1];

  if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
    return std::nullopt;
  }

  int col = file - 'a';
  int row = 8 - (rank - '0');

  Position pos{row, col};
  if (!pos.isValid()) {
    return std::nullopt;
  }

  return pos;
}
