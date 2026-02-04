#pragma once

struct Position {
  int row = 0;
  int col = 0;

  bool isValid() const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
  }

  bool operator==(const Position& other) const {
    return row == other.row && col == other.col;
  }

  bool operator!=(const Position& other) const {
    return !(*this == other);
  }

  bool operator<(const Position& other) const {
    if (row != other.row) {
      return row < other.row;
    }
    return col < other.col;
  }
};
