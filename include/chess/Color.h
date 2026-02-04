#pragma once

enum class Color {
  White,
  Black
};

inline Color opposite(Color color) {
  return (color == Color::White) ? Color::Black : Color::White;
}
