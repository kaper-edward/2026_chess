#include "chess/CheckDetector.h"

#include <array>
#include <optional>

namespace {
std::optional<Position> findKing(const Board& board, Color color) {
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      const Piece* piece = board.pieceAt({row, col});
      if (piece && piece->type == PieceType::King && piece->color == color) {
        return Position{row, col};
      }
    }
  }
  return std::nullopt;
}

bool isEnemyPiece(const Board& board, Position pos, Color enemy, PieceType type) {
  const Piece* piece = board.pieceAt(pos);
  return piece && piece->color == enemy && piece->type == type;
}
}  // namespace

bool CheckDetector::inCheck(const Board& board, Color color) const {
  auto kingPosOpt = findKing(board, color);
  if (!kingPosOpt) {
    return false;
  }
  Position kingPos = *kingPosOpt;
  Color enemy = opposite(color);

  int pawnRowOffset = (enemy == Color::White) ? 1 : -1;
  Position pawnLeft{kingPos.row + pawnRowOffset, kingPos.col - 1};
  Position pawnRight{kingPos.row + pawnRowOffset, kingPos.col + 1};
  if (Board::isInside(pawnLeft) && isEnemyPiece(board, pawnLeft, enemy, PieceType::Pawn)) {
    return true;
  }
  if (Board::isInside(pawnRight) && isEnemyPiece(board, pawnRight, enemy, PieceType::Pawn)) {
    return true;
  }

  static constexpr std::array<std::pair<int, int>, 8> knightOffsets = {
      {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}}};
  for (const auto& [dr, dc] : knightOffsets) {
    Position pos{kingPos.row + dr, kingPos.col + dc};
    if (Board::isInside(pos) && isEnemyPiece(board, pos, enemy, PieceType::Knight)) {
      return true;
    }
  }

  for (int dr = -1; dr <= 1; ++dr) {
    for (int dc = -1; dc <= 1; ++dc) {
      if (dr == 0 && dc == 0) {
        continue;
      }
      Position pos{kingPos.row + dr, kingPos.col + dc};
      if (Board::isInside(pos) && isEnemyPiece(board, pos, enemy, PieceType::King)) {
        return true;
      }
    }
  }

  static constexpr std::array<std::pair<int, int>, 4> rookDirs = {
      {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
  for (const auto& [dr, dc] : rookDirs) {
    int row = kingPos.row + dr;
    int col = kingPos.col + dc;
    while (Board::isInside({row, col})) {
      const Piece* piece = board.pieceAt({row, col});
      if (piece) {
        if (piece->color == enemy && (piece->type == PieceType::Rook || piece->type == PieceType::Queen)) {
          return true;
        }
        break;
      }
      row += dr;
      col += dc;
    }
  }

  static constexpr std::array<std::pair<int, int>, 4> bishopDirs = {
      {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};
  for (const auto& [dr, dc] : bishopDirs) {
    int row = kingPos.row + dr;
    int col = kingPos.col + dc;
    while (Board::isInside({row, col})) {
      const Piece* piece = board.pieceAt({row, col});
      if (piece) {
        if (piece->color == enemy && (piece->type == PieceType::Bishop || piece->type == PieceType::Queen)) {
          return true;
        }
        break;
      }
      row += dr;
      col += dc;
    }
  }

  return false;
}
