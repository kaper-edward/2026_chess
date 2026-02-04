#include "chess/GameState.h"

#include <algorithm>
#include <cassert>

namespace {
bool containsMove(const std::vector<Position>& moves, Position target) {
  return std::find(moves.begin(), moves.end(), target) != moves.end();
}
}  // namespace

GameState::GameState(std::unique_ptr<IMoveRules> moveRules,
                     std::unique_ptr<ICheckDetector> checkDetector)
    : board_(),
      turn_(Color::White),
      moveRules_(std::move(moveRules)),
      checkDetector_(std::move(checkDetector)) {
  assert(moveRules_ && "IMoveRules must not be null");
  assert(checkDetector_ && "ICheckDetector must not be null");
}

GameState::GameState(Board board, Color turn, std::unique_ptr<IMoveRules> moveRules,
                     std::unique_ptr<ICheckDetector> checkDetector)
    : board_(std::move(board)),
      turn_(turn),
      moveRules_(std::move(moveRules)),
      checkDetector_(std::move(checkDetector)) {
  assert(moveRules_ && "IMoveRules must not be null");
  assert(checkDetector_ && "ICheckDetector must not be null");
}

GameState GameState::Standard(std::unique_ptr<IMoveRules> moveRules,
                              std::unique_ptr<ICheckDetector> checkDetector) {
  return GameState(Board::StandardSetup(), Color::White, std::move(moveRules),
                   std::move(checkDetector));
}

GameStatus GameState::status() const {
  return hasAnyLegalMove(turn_) ? GameStatus::Ongoing : GameStatus::NoLegalMoves;
}

bool GameState::tryMove(Position from, Position to) {
  if (!Board::isInside(from) || !Board::isInside(to)) {
    return false;
  }

  const Piece* piece = board_.pieceAt(from);
  if (piece == nullptr || piece->color != turn_) {
    return false;
  }

  const Piece* target = board_.pieceAt(to);
  // Defense-in-depth: pseudoLegalMoves also excludes same-color targets.
  if (target && target->color == piece->color) {
    return false;
  }

  const auto moves = moveRules_->pseudoLegalMoves(board_, from);
  if (!containsMove(moves, to)) {
    return false;
  }

  Board next = board_;
  next.movePiece(from, to);
  if (checkDetector_->inCheck(next, turn_)) {
    return false;
  }

  board_ = std::move(next);
  turn_ = opposite(turn_);
  return true;
}

bool GameState::hasAnyLegalMove(Color color) const {
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      Position from{row, col};
      const Piece* piece = board_.pieceAt(from);
      if (!piece || piece->color != color) {
        continue;
      }

      const auto moves = moveRules_->pseudoLegalMoves(board_, from);
      for (const auto& to : moves) {
        Board next = board_;
        next.movePiece(from, to);
        if (!checkDetector_->inCheck(next, color)) {
          return true;
        }
      }
    }
  }
  return false;
}
