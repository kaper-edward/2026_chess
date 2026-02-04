#include <gtest/gtest.h>

#include "chess/CheckDetector.h"
#include "chess/GameController.h"
#include "chess/MoveRules.h"

TEST(GameController, RejectsInvalidCoordinate) {
  GameController controller(std::make_unique<MoveRules>(),
                            std::make_unique<CheckDetector>());

  EXPECT_FALSE(controller.makeMove("z9", "a1"));
  EXPECT_FALSE(controller.makeMove("a2", "z9"));
}

TEST(GameController, AppliesValidMove) {
  GameController controller(std::make_unique<MoveRules>(),
                            std::make_unique<CheckDetector>());

  EXPECT_EQ(controller.turn(), Color::White);
  EXPECT_TRUE(controller.makeMove("a2", "a3"));
  EXPECT_EQ(controller.turn(), Color::Black);
}

TEST(GameController, StatusIsOngoingAtStart) {
  GameController controller(std::make_unique<MoveRules>(),
                            std::make_unique<CheckDetector>());
  EXPECT_EQ(controller.status(), GameStatus::Ongoing);
}

TEST(GameController, RejectsIllegalMoveWithValidCoordinates) {
  GameController controller(std::make_unique<MoveRules>(),
                            std::make_unique<CheckDetector>());
  EXPECT_FALSE(controller.makeMove("a2", "a5"));
  EXPECT_EQ(controller.turn(), Color::White);
}
