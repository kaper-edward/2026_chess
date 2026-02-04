#pragma once

#include <memory>

#include "chess/Board.h"
#include "chess/ICheckDetector.h"
#include "chess/IMoveRules.h"

class GameState {
 public:
  GameState(std::unique_ptr<IMoveRules> moveRules, std::unique_ptr<ICheckDetector> checkDetector);
  GameState(Board board, Color turn, std::unique_ptr<IMoveRules> moveRules,
            std::unique_ptr<ICheckDetector> checkDetector);

  static GameState Standard(std::unique_ptr<IMoveRules> moveRules,
                            std::unique_ptr<ICheckDetector> checkDetector);

  const Board& board() const { return board_; }
  Color turn() const { return turn_; }

  bool tryMove(Position from, Position to);

 private:
  Board board_{};
  Color turn_ = Color::White;
  std::unique_ptr<IMoveRules> moveRules_;
  std::unique_ptr<ICheckDetector> checkDetector_;
};
