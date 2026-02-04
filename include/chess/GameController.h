#pragma once

#include <memory>

#include "chess/IGameController.h"
#include "chess/ICheckDetector.h"
#include "chess/IMoveRules.h"
#include "chess/GameState.h"

class GameController final : public IGameController {
 public:
  GameController(std::unique_ptr<IMoveRules> moveRules,
                 std::unique_ptr<ICheckDetector> checkDetector);

  bool makeMove(const std::string& from, const std::string& to) override;
  GameStatus status() const override;
  Color turn() const override;
  const Board& board() const override;

 private:
  GameState state_;
};
