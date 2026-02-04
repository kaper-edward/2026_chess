#include "chess/GameController.h"

#include "chess/CoordinateParser.h"

GameController::GameController(std::unique_ptr<IMoveRules> moveRules,
                               std::unique_ptr<ICheckDetector> checkDetector)
    : state_(GameState::Standard(std::move(moveRules), std::move(checkDetector))) {}

bool GameController::makeMove(const std::string& from, const std::string& to) {
  auto fromPos = CoordinateParser::parse(from);
  if (!fromPos) {
    return false;
  }
  auto toPos = CoordinateParser::parse(to);
  if (!toPos) {
    return false;
  }
  return state_.tryMove(*fromPos, *toPos);
}

GameStatus GameController::status() const {
  return state_.status();
}

Color GameController::turn() const {
  return state_.turn();
}

const Board& GameController::board() const {
  return state_.board();
}

std::vector<Position> GameController::legalMovesFrom(const std::string& square) const {
  auto pos = CoordinateParser::parse(square);
  if (!pos) {
    return {};
  }
  return state_.legalMovesFrom(*pos);
}
