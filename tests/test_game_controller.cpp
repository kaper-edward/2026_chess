#include <gtest/gtest.h>

#include "chess/CheckDetector.h"
#include "chess/GameController.h"
#include "chess/MoveRules.h"

TEST(GameController, RejectsInvalidCoordinate) {
  GameController controller(std::make_unique<MoveRules>(),
                            std::make_unique<CheckDetector>());

  EXPECT_FALSE(controller.MakeMove("z9", "a1"));
  EXPECT_FALSE(controller.MakeMove("a2", "z9"));
}

TEST(GameController, AppliesValidMove) {
  GameController controller(std::make_unique<MoveRules>(),
                            std::make_unique<CheckDetector>());

  EXPECT_EQ(controller.Turn(), Color::White);
  EXPECT_TRUE(controller.MakeMove("a2", "a3"));
  EXPECT_EQ(controller.Turn(), Color::Black);
}
