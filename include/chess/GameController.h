#pragma once

#include <memory>

#include "chess/IGameController.h"
#include "chess/ICheckDetector.h"
#include "chess/IMoveRules.h"

class GameController final : public IGameController {
 public:
  GameController(std::unique_ptr<IMoveRules> moveRules,
                 std::unique_ptr<ICheckDetector> checkDetector);

  bool MakeMove(const std::string& from, const std::string& to) override;
  GameStatus Status() const override;
  Color Turn() const override;

 private:
  GameState state_;
};
