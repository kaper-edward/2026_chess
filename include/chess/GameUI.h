#pragma once

#include <memory>

#include "chess/IBoardRenderer.h"
#include "chess/IGameController.h"
#include "chess/IInputProvider.h"
#include "chess/RenderState.h"

class GameUI {
 public:
  GameUI(std::unique_ptr<IGameController> controller,
         std::unique_ptr<IInputProvider> input,
         std::unique_ptr<IBoardRenderer> renderer);

  void run();

 private:
  void handleSelect(const std::string& square);
  void handleCommand(const std::string& command);
  void clearSelection();

  std::unique_ptr<IGameController> controller_;
  std::unique_ptr<IInputProvider> input_;
  std::unique_ptr<IBoardRenderer> renderer_;
  RenderState renderState_;
};
