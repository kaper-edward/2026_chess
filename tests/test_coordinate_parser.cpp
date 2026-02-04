#include <gtest/gtest.h>

#include "chess/CoordinateParser.h"

TEST(CoordinateParser, ParsesValidCoordinates) {
  auto pos = CoordinateParser::parse("a1");
  ASSERT_TRUE(pos.has_value());
  EXPECT_EQ(pos->row, 7);
  EXPECT_EQ(pos->col, 0);

  auto pos2 = CoordinateParser::parse("h8");
  ASSERT_TRUE(pos2.has_value());
  EXPECT_EQ(pos2->row, 0);
  EXPECT_EQ(pos2->col, 7);
}

TEST(CoordinateParser, AcceptsUppercaseFile) {
  auto pos = CoordinateParser::parse("E2");
  ASSERT_TRUE(pos.has_value());
  EXPECT_EQ(pos->row, 6);
  EXPECT_EQ(pos->col, 4);
}

TEST(CoordinateParser, RejectsInvalidInput) {
  EXPECT_FALSE(CoordinateParser::parse("i2").has_value());
  EXPECT_FALSE(CoordinateParser::parse("a9").has_value());
  EXPECT_FALSE(CoordinateParser::parse("a0").has_value());
  EXPECT_FALSE(CoordinateParser::parse("a10").has_value());
  EXPECT_FALSE(CoordinateParser::parse("").has_value());
}
