#include <gtest/gtest.h>

#include <utility>

#include "chess/GameUI.h"

namespace {
class FakeInputProvider : public IInputProvider {
 public:
  explicit FakeInputProvider(std::vector<InputEvent> events)
      : events_(std::move(events)) {}

  InputEvent poll() override {
    if (index_ < events_.size()) {
      return events_[index_++];
    }
    return {InputEvent::Type::Quit, ""};
  }

 private:
  std::vector<InputEvent> events_;
  std::size_t index_ = 0;
};

class FakeRenderer : public IBoardRenderer {
 public:
  void render(const Board&, const RenderState& state) override {
    lastState = state;
    renderCalls++;
  }

  RenderState lastState;
  int renderCalls = 0;
};

class FakeGameController : public IGameController {
 public:
  explicit FakeGameController(Board board) : board_(std::move(board)) {}

  bool makeMove(const std::string& from, const std::string& to) override {
    lastMoveFrom_ = from;
    lastMoveTo_ = to;
    moveCalls_++;
    if (moveResult_) {
      turn_ = opposite(turn_);
    }
    return moveResult_;
  }

  GameStatus status() const override { return status_; }

  Color turn() const override { return turn_; }

  const Board& board() const override { return board_; }

  std::vector<Position> legalMovesFrom(const std::string& square) const override {
    lastLegalMovesSquare_ = square;
    return legalMoves_;
  }

  void setLegalMoves(std::vector<Position> moves) { legalMoves_ = std::move(moves); }
  void setMoveResult(bool result) { moveResult_ = result; }

  const std::string& lastMoveFrom() const { return lastMoveFrom_; }
  const std::string& lastMoveTo() const { return lastMoveTo_; }
  const std::string& lastLegalMovesSquare() const { return lastLegalMovesSquare_; }
  int moveCalls() const { return moveCalls_; }

 private:
  Board board_{};
  mutable std::string lastLegalMovesSquare_;
  std::string lastMoveFrom_;
  std::string lastMoveTo_;
  int moveCalls_ = 0;
  bool moveResult_ = false;
  std::vector<Position> legalMoves_;
  Color turn_ = Color::White;
  GameStatus status_ = GameStatus::Ongoing;
};
}  // namespace

TEST(GameUI, SelectAndMoveClearsSelectionOnSuccess) {
  auto controller = std::make_unique<FakeGameController>(Board::StandardSetup());
  controller->setMoveResult(true);
  controller->setLegalMoves({{5, 0}});

  FakeGameController* controllerPtr = controller.get();
  auto input = std::make_unique<FakeInputProvider>(std::vector<InputEvent>{
      {InputEvent::Type::Select, "a2"},
      {InputEvent::Type::Select, "a3"},
      {InputEvent::Type::Quit, ""}});
  auto renderer = std::make_unique<FakeRenderer>();
  FakeRenderer* rendererPtr = renderer.get();

  GameUI ui(std::move(controller), std::move(input), std::move(renderer));
  ui.run();

  EXPECT_EQ(controllerPtr->lastMoveFrom(), "a2");
  EXPECT_EQ(controllerPtr->lastMoveTo(), "a3");
  EXPECT_EQ(controllerPtr->moveCalls(), 1);
  EXPECT_FALSE(rendererPtr->lastState.selected.has_value());
  EXPECT_TRUE(rendererPtr->lastState.legalMoves.empty());
}

TEST(GameUI, CommandMoveCallsController) {
  auto controller = std::make_unique<FakeGameController>(Board::StandardSetup());
  controller->setMoveResult(true);

  FakeGameController* controllerPtr = controller.get();
  auto input = std::make_unique<FakeInputProvider>(std::vector<InputEvent>{
      {InputEvent::Type::Command, "a2a3"},
      {InputEvent::Type::Quit, ""}});
  auto renderer = std::make_unique<FakeRenderer>();

  GameUI ui(std::move(controller), std::move(input), std::move(renderer));
  ui.run();

  EXPECT_EQ(controllerPtr->lastMoveFrom(), "a2");
  EXPECT_EQ(controllerPtr->lastMoveTo(), "a3");
  EXPECT_EQ(controllerPtr->moveCalls(), 1);
}

TEST(GameUI, SelectUpdatesLegalMoves) {
  auto controller = std::make_unique<FakeGameController>(Board::StandardSetup());
  controller->setLegalMoves({{5, 0}, {4, 0}});

  FakeGameController* controllerPtr = controller.get();
  auto input = std::make_unique<FakeInputProvider>(std::vector<InputEvent>{
      {InputEvent::Type::Select, "a2"},
      {InputEvent::Type::Quit, ""}});
  auto renderer = std::make_unique<FakeRenderer>();
  FakeRenderer* rendererPtr = renderer.get();

  GameUI ui(std::move(controller), std::move(input), std::move(renderer));
  ui.run();

  EXPECT_EQ(controllerPtr->lastLegalMovesSquare(), "a2");
  EXPECT_TRUE(rendererPtr->lastState.selected.has_value());
  EXPECT_EQ(rendererPtr->lastState.legalMoves.size(), 2u);
}
