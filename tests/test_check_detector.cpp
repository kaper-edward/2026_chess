#include <gtest/gtest.h>

#include "chess/CheckDetector.h"
#include "chess/GameState.h"
#include "chess/MoveRules.h"

TEST(CheckDetector, DetectsRookCheck) {
  Board board;
  board.placePiece({4, 4}, {PieceType::King, Color::White});
  board.placePiece({0, 4}, {PieceType::King, Color::Black});
  board.placePiece({4, 7}, {PieceType::Rook, Color::Black});

  CheckDetector detector;
  EXPECT_TRUE(detector.inCheck(board, Color::White));
  EXPECT_FALSE(detector.inCheck(board, Color::Black));
}

TEST(CheckDetector, DetectsKnightCheck) {
  Board board;
  board.placePiece({4, 4}, {PieceType::King, Color::White});
  board.placePiece({6, 5}, {PieceType::Knight, Color::Black});

  CheckDetector detector;
  EXPECT_TRUE(detector.inCheck(board, Color::White));
}

TEST(CheckDetector, DetectsPawnCheck) {
  Board board;
  board.placePiece({4, 4}, {PieceType::King, Color::White});
  board.placePiece({3, 3}, {PieceType::Pawn, Color::Black});

  CheckDetector detector;
  EXPECT_TRUE(detector.inCheck(board, Color::White));
}

TEST(CheckDetector, DetectsBishopDiagonalCheck) {
  Board board;
  board.placePiece({4, 4}, {PieceType::King, Color::White});
  board.placePiece({1, 1}, {PieceType::Bishop, Color::Black});

  CheckDetector detector;
  EXPECT_TRUE(detector.inCheck(board, Color::White));
}

TEST(CheckDetector, DetectsAdjacentKingCheck) {
  Board board;
  board.placePiece({4, 4}, {PieceType::King, Color::White});
  board.placePiece({5, 5}, {PieceType::King, Color::Black});

  CheckDetector detector;
  EXPECT_TRUE(detector.inCheck(board, Color::White));
}

TEST(GameState, RejectsMoveThatLeavesKingInCheck) {
  Board board;
  board.placePiece({7, 4}, {PieceType::King, Color::White});
  board.placePiece({7, 5}, {PieceType::Rook, Color::White});
  board.placePiece({7, 7}, {PieceType::Rook, Color::Black});

  GameState game(board, Color::White,
                 std::make_unique<MoveRules>(),
                 std::make_unique<CheckDetector>());

  EXPECT_FALSE(game.tryMove({7, 5}, {6, 5}));
}

TEST(GameState, AllowsMoveThatResolvesCheck) {
  Board board;
  board.placePiece({7, 4}, {PieceType::King, Color::White});
  board.placePiece({7, 7}, {PieceType::Rook, Color::Black});
  board.placePiece({6, 5}, {PieceType::Rook, Color::White});

  GameState game(board, Color::White,
                 std::make_unique<MoveRules>(),
                 std::make_unique<CheckDetector>());

  EXPECT_TRUE(game.tryMove({6, 5}, {7, 5}));
}
