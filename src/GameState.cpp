#include "chess/GameState.h"

#include <algorithm>
#include <cassert>

namespace {
bool containsMove(const std::vector<Position>& moves, Position target) {
  return std::find(moves.begin(), moves.end(), target) != moves.end();
}
}  // namespace

GameState::GameState(std::unique_ptr<IMoveRules> moveRules)
    : board_(), turn_(Color::White), moveRules_(std::move(moveRules)) {
  assert(moveRules_ && "IMoveRules must not be null");
}

GameState::GameState(Board board, Color turn, std::unique_ptr<IMoveRules> moveRules)
    : board_(std::move(board)), turn_(turn), moveRules_(std::move(moveRules)) {
  assert(moveRules_ && "IMoveRules must not be null");
}

GameState GameState::Standard(std::unique_ptr<IMoveRules> moveRules) {
  return GameState(Board::StandardSetup(), Color::White, std::move(moveRules));
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

  board_.movePiece(from, to);
  turn_ = opposite(turn_);
  return true;
}
