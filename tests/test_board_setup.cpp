#include <gtest/gtest.h>

#include "chess/Board.h"

TEST(BoardSetup, StandardSetupPlacesKingsAndQueens) {
  Board board = Board::StandardSetup();

  const Piece* whiteKing = board.pieceAt({7, 4});
  ASSERT_NE(whiteKing, nullptr);
  EXPECT_EQ(whiteKing->type, PieceType::King);
  EXPECT_EQ(whiteKing->color, Color::White);

  const Piece* blackKing = board.pieceAt({0, 4});
  ASSERT_NE(blackKing, nullptr);
  EXPECT_EQ(blackKing->type, PieceType::King);
  EXPECT_EQ(blackKing->color, Color::Black);

  const Piece* whiteQueen = board.pieceAt({7, 3});
  ASSERT_NE(whiteQueen, nullptr);
  EXPECT_EQ(whiteQueen->type, PieceType::Queen);
  EXPECT_EQ(whiteQueen->color, Color::White);

  const Piece* blackQueen = board.pieceAt({0, 3});
  ASSERT_NE(blackQueen, nullptr);
  EXPECT_EQ(blackQueen->type, PieceType::Queen);
  EXPECT_EQ(blackQueen->color, Color::Black);
}

TEST(BoardSetup, StandardSetupPlacesPawns) {
  Board board = Board::StandardSetup();

  for (int col = 0; col < 8; ++col) {
    const Piece* whitePawn = board.pieceAt({6, col});
    ASSERT_NE(whitePawn, nullptr);
    EXPECT_EQ(whitePawn->type, PieceType::Pawn);
    EXPECT_EQ(whitePawn->color, Color::White);

    const Piece* blackPawn = board.pieceAt({1, col});
    ASSERT_NE(blackPawn, nullptr);
    EXPECT_EQ(blackPawn->type, PieceType::Pawn);
    EXPECT_EQ(blackPawn->color, Color::Black);
  }
}

TEST(BoardSetup, EmptySquareIsEmpty) {
  Board board = Board::StandardSetup();
  EXPECT_TRUE(board.isEmpty({3, 3}));
}
