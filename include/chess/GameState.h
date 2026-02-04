#pragma once

#include <memory>
#include <vector>

#include "chess/Board.h"
#include "chess/ICheckDetector.h"
#include "chess/IMoveRules.h"
#include "chess/GameStatus.h"

class GameState {
 public:
  GameState(std::unique_ptr<IMoveRules> moveRules, std::unique_ptr<ICheckDetector> checkDetector);
  GameState(Board board, Color turn, std::unique_ptr<IMoveRules> moveRules,
            std::unique_ptr<ICheckDetector> checkDetector);

  static GameState Standard(std::unique_ptr<IMoveRules> moveRules,
                            std::unique_ptr<ICheckDetector> checkDetector);

  const Board& board() const { return board_; }
  Color turn() const { return turn_; }

  std::vector<Position> legalMovesFrom(Position from) const;
  GameStatus status() const;
  bool tryMove(Position from, Position to);

 private:
  std::vector<Position> legalMovesFromInternal(Position from, Color color) const;
  bool hasAnyLegalMove(Color color) const;

  Board board_{};
  Color turn_ = Color::White;
  std::unique_ptr<IMoveRules> moveRules_;
  std::unique_ptr<ICheckDetector> checkDetector_;
};
