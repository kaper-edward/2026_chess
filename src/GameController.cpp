#include "chess/GameController.h"

#include "chess/CoordinateParser.h"

GameController::GameController(std::unique_ptr<IMoveRules> moveRules,
                               std::unique_ptr<ICheckDetector> checkDetector)
    : state_(GameState::Standard(std::move(moveRules), std::move(checkDetector))) {}

bool GameController::MakeMove(const std::string& from, const std::string& to) {
  auto fromPos = CoordinateParser::Parse(from);
  if (!fromPos) {
    return false;
  }
  auto toPos = CoordinateParser::Parse(to);
  if (!toPos) {
    return false;
  }
  return state_.tryMove(*fromPos, *toPos);
}

GameStatus GameController::Status() const {
  return state_.status();
}

Color GameController::Turn() const {
  return state_.turn();
}
