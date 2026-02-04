#include "chess/CheckDetector.h"

#include <array>
#include <cassert>
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

bool isAttackedAlongDirs(const Board& board, Position start, Color enemy,
                          const std::array<std::pair<int, int>, 4>& dirs,
                          PieceType typeA, PieceType typeB) {
  for (const auto& [dr, dc] : dirs) {
    int row = start.row + dr;
    int col = start.col + dc;
    while (Board::isInside({row, col})) {
      const Piece* piece = board.pieceAt({row, col});
      if (piece) {
        if (piece->color == enemy && (piece->type == typeA || piece->type == typeB)) {
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
}  // namespace

bool CheckDetector::inCheck(const Board& board, Color color) const {
  auto kingPosOpt = findKing(board, color);
  assert(kingPosOpt.has_value() && "Board missing king");
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
  if (isAttackedAlongDirs(board, kingPos, enemy, rookDirs, PieceType::Rook,
                          PieceType::Queen)) {
    return true;
  }

  static constexpr std::array<std::pair<int, int>, 4> bishopDirs = {
      {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};
  if (isAttackedAlongDirs(board, kingPos, enemy, bishopDirs, PieceType::Bishop,
                          PieceType::Queen)) {
    return true;
  }

  return false;
}
