#pragma once

#include <array>
#include <optional>
#include "chess/Piece.h"
#include "chess/Position.h"

class Board {
 public:
  Board();

  static Board StandardSetup();

  const Piece* pieceAt(Position pos) const;
  Piece* pieceAt(Position pos);
  bool isEmpty(Position pos) const;

  void placePiece(Position pos, const Piece& piece);
  void clearPiece(Position pos);
  void movePiece(Position from, Position to);

  static bool isInside(Position pos);

 private:
  std::array<std::array<std::optional<Piece>, 8>, 8> squares_{};
};
