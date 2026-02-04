#include "chess/GameUI.h"

#include <sstream>

#include "chess/CoordinateParser.h"

namespace {
std::optional<std::pair<std::string, std::string>> parseCommand(const std::string& command) {
  if (command.size() == 4) {
    return std::make_pair(command.substr(0, 2), command.substr(2, 2));
  }

  std::istringstream iss(command);
  std::string from;
  std::string to;
  if (!(iss >> from >> to)) {
    return std::nullopt;
  }
  std::string extra;
  if (iss >> extra) {
    return std::nullopt;
  }
  return std::make_pair(from, to);
}

std::string toCoord(Position pos) {
  char file = static_cast<char>('a' + pos.col);
  char rank = static_cast<char>('8' - pos.row);
  return std::string{file, rank};
}
}  // namespace

GameUI::GameUI(std::unique_ptr<IGameController> controller,
               std::unique_ptr<IInputProvider> input,
               std::unique_ptr<IBoardRenderer> renderer)
    : controller_(std::move(controller)),
      input_(std::move(input)),
      renderer_(std::move(renderer)),
      renderState_() {}

void GameUI::run() {
  bool running = true;
  while (running) {
    InputEvent event = input_->poll();
    switch (event.type) {
      case InputEvent::Type::Quit:
        running = false;
        break;
      case InputEvent::Type::Select:
        handleSelect(event.payload);
        break;
      case InputEvent::Type::Command:
        handleCommand(event.payload);
        break;
      case InputEvent::Type::None:
        break;
    }

    renderer_->render(controller_->board(), renderState_);
  }
}

void GameUI::handleSelect(const std::string& square) {
  auto posOpt = CoordinateParser::parse(square);
  if (!posOpt) {
    return;
  }

  if (renderState_.selected && *renderState_.selected == *posOpt) {
    clearSelection();
    return;
  }

  if (!renderState_.selected) {
    renderState_.selected = *posOpt;
    renderState_.legalMoves = controller_->legalMovesFrom(square);
    return;
  }

  std::string from = toCoord(*renderState_.selected);
  std::string to = toCoord(*posOpt);
  if (controller_->makeMove(from, to)) {
    clearSelection();
    return;
  }

  renderState_.selected = *posOpt;
  renderState_.legalMoves = controller_->legalMovesFrom(to);
}

void GameUI::handleCommand(const std::string& command) {
  auto parsed = parseCommand(command);
  if (!parsed) {
    return;
  }

  controller_->makeMove(parsed->first, parsed->second);
  clearSelection();
}

void GameUI::clearSelection() {
  renderState_.selected.reset();
  renderState_.legalMoves.clear();
}
