#pragma once

#include <memory>

#include "chess/Board.h"
#include "chess/IMoveRules.h"

class GameState {
 public:
  explicit GameState(std::unique_ptr<IMoveRules> moveRules);
  GameState(Board board, Color turn, std::unique_ptr<IMoveRules> moveRules);

  static GameState Standard(std::unique_ptr<IMoveRules> moveRules);

  const Board& board() const { return board_; }
  Color turn() const { return turn_; }

  bool tryMove(Position from, Position to);

 private:
  Board board_{};
  Color turn_ = Color::White;
  std::unique_ptr<IMoveRules> moveRules_;
};
