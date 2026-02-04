#include "chess/MoveRules.h"

#include <array>

namespace {
Color opposite(Color color) {
  return (color == Color::White) ? Color::Black : Color::White;
}

void addIfValid(const Board& board, const Piece& piece, Position pos,
               std::vector<Position>& moves) {
  if (!Board::isInside(pos)) {
    return;
  }
  const Piece* target = board.pieceAt(pos);
  if (target == nullptr || target->color != piece.color) {
    moves.push_back(pos);
  }
}

void addRay(const Board& board, const Piece& piece, Position from,
            int dRow, int dCol, std::vector<Position>& moves) {
  int row = from.row + dRow;
  int col = from.col + dCol;
  while (Board::isInside({row, col})) {
    Position pos{row, col};
    const Piece* target = board.pieceAt(pos);
    if (target == nullptr) {
      moves.push_back(pos);
    } else {
      if (target->color != piece.color) {
        moves.push_back(pos);
      }
      break;
    }
    row += dRow;
    col += dCol;
  }
}
}  // namespace

std::vector<Position> MoveRules::pseudoLegalMoves(const Board& board, Position from) const {
  std::vector<Position> moves;
  if (!Board::isInside(from)) {
    return moves;
  }

  const Piece* piece = board.pieceAt(from);
  if (piece == nullptr) {
    return moves;
  }

  switch (piece->type) {
    case PieceType::King: {
      for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
          if (dr == 0 && dc == 0) {
            continue;
          }
          addIfValid(board, *piece, {from.row + dr, from.col + dc}, moves);
        }
      }
      break;
    }
    case PieceType::Queen: {
      constexpr std::array<std::pair<int, int>, 8> directions = {
          {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};
      for (const auto& [dr, dc] : directions) {
        addRay(board, *piece, from, dr, dc, moves);
      }
      break;
    }
    case PieceType::Rook: {
      constexpr std::array<std::pair<int, int>, 4> directions = {
          {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
      for (const auto& [dr, dc] : directions) {
        addRay(board, *piece, from, dr, dc, moves);
      }
      break;
    }
    case PieceType::Bishop: {
      constexpr std::array<std::pair<int, int>, 4> directions = {
          {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};
      for (const auto& [dr, dc] : directions) {
        addRay(board, *piece, from, dr, dc, moves);
      }
      break;
    }
    case PieceType::Knight: {
      constexpr std::array<std::pair<int, int>, 8> jumps = {
          {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}}};
      for (const auto& [dr, dc] : jumps) {
        addIfValid(board, *piece, {from.row + dr, from.col + dc}, moves);
      }
      break;
    }
    case PieceType::Pawn: {
      int dir = (piece->color == Color::White) ? -1 : 1;
      int startRow = (piece->color == Color::White) ? 6 : 1;

      Position oneStep{from.row + dir, from.col};
      if (Board::isInside(oneStep) && board.isEmpty(oneStep)) {
        moves.push_back(oneStep);
        Position twoStep{from.row + 2 * dir, from.col};
        if (from.row == startRow && board.isEmpty(twoStep)) {
          moves.push_back(twoStep);
        }
      }

      Position captureLeft{from.row + dir, from.col - 1};
      Position captureRight{from.row + dir, from.col + 1};
      const Piece* leftTarget = board.pieceAt(captureLeft);
      if (leftTarget && leftTarget->color == opposite(piece->color)) {
        moves.push_back(captureLeft);
      }
      const Piece* rightTarget = board.pieceAt(captureRight);
      if (rightTarget && rightTarget->color == opposite(piece->color)) {
        moves.push_back(captureRight);
      }
      break;
    }
  }

  return moves;
}
