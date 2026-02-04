#include "chess/Board.h"

#include <stdexcept>

namespace {
void placeBackRank(Board& board, Color color, int row) {
  board.placePiece({row, 0}, {PieceType::Rook, color});
  board.placePiece({row, 1}, {PieceType::Knight, color});
  board.placePiece({row, 2}, {PieceType::Bishop, color});
  board.placePiece({row, 3}, {PieceType::Queen, color});
  board.placePiece({row, 4}, {PieceType::King, color});
  board.placePiece({row, 5}, {PieceType::Bishop, color});
  board.placePiece({row, 6}, {PieceType::Knight, color});
  board.placePiece({row, 7}, {PieceType::Rook, color});
}
}  // namespace

Board::Board() : squares_{} {}

Board Board::StandardSetup() {
  Board board;
  placeBackRank(board, Color::Black, 0);
  placeBackRank(board, Color::White, 7);

  for (int col = 0; col < 8; ++col) {
    board.placePiece({1, col}, {PieceType::Pawn, Color::Black});
    board.placePiece({6, col}, {PieceType::Pawn, Color::White});
  }

  return board;
}

const Piece* Board::pieceAt(Position pos) const {
  if (!isInside(pos)) {
    return nullptr;
  }
  const auto& cell = squares_[pos.row][pos.col];
  return cell ? &(*cell) : nullptr;
}

Piece* Board::pieceAt(Position pos) {
  if (!isInside(pos)) {
    return nullptr;
  }
  auto& cell = squares_[pos.row][pos.col];
  return cell ? &(*cell) : nullptr;
}

bool Board::isEmpty(Position pos) const {
  return pieceAt(pos) == nullptr;
}

void Board::placePiece(Position pos, const Piece& piece) {
  if (!isInside(pos)) {
    throw std::out_of_range("Position outside board");
  }
  squares_[pos.row][pos.col] = piece;
}

void Board::clearPiece(Position pos) {
  if (!isInside(pos)) {
    throw std::out_of_range("Position outside board");
  }
  squares_[pos.row][pos.col].reset();
}

void Board::movePiece(Position from, Position to) {
  if (!isInside(from) || !isInside(to)) {
    throw std::out_of_range("Position outside board");
  }
  auto& src = squares_[from.row][from.col];
  if (!src.has_value()) {
    throw std::runtime_error("No piece at source position");
  }
  squares_[to.row][to.col] = *src;
  src.reset();
}

bool Board::isInside(Position pos) {
  return pos.isValid();
}
