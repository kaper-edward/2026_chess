#include <gtest/gtest.h>

#include <set>
#include <utility>

#include "chess/Board.h"
#include "chess/MoveRules.h"

namespace {
std::set<std::pair<int, int>> ToSet(const std::vector<Position>& moves) {
  std::set<std::pair<int, int>> result;
  for (const auto& move : moves) {
    result.insert({move.row, move.col});
  }
  return result;
}
}  // namespace

TEST(MoveRules, KnightJumpsOverPieces) {
  Board board = Board::StandardSetup();
  auto moves = MoveRules::legalMoves(board, {7, 1});

  std::set<std::pair<int, int>> expected = {{5, 0}, {5, 2}};
  EXPECT_EQ(ToSet(moves), expected);
}

TEST(MoveRules, BishopBlockedAtStart) {
  Board board = Board::StandardSetup();
  auto moves = MoveRules::legalMoves(board, {7, 2});
  EXPECT_TRUE(moves.empty());
}

TEST(MoveRules, PawnInitialMoves) {
  Board board = Board::StandardSetup();
  auto moves = MoveRules::legalMoves(board, {6, 0});

  std::set<std::pair<int, int>> expected = {{5, 0}, {4, 0}};
  EXPECT_EQ(ToSet(moves), expected);
}

TEST(MoveRules, PawnCapturesDiagonal) {
  Board board;
  board.placePiece({4, 4}, {PieceType::Pawn, Color::White});
  board.placePiece({3, 3}, {PieceType::Knight, Color::Black});
  board.placePiece({3, 5}, {PieceType::Bishop, Color::Black});

  auto moves = MoveRules::legalMoves(board, {4, 4});
  std::set<std::pair<int, int>> expected = {{3, 4}, {3, 3}, {3, 5}};
  EXPECT_EQ(ToSet(moves), expected);
}

TEST(MoveRules, RookStopsAtBlockingPiece) {
  Board board;
  board.placePiece({4, 4}, {PieceType::Rook, Color::White});
  board.placePiece({4, 6}, {PieceType::Knight, Color::White});
  board.placePiece({4, 2}, {PieceType::Knight, Color::Black});

  auto moves = MoveRules::legalMoves(board, {4, 4});
  std::set<std::pair<int, int>> expected = {
      {4, 5}, {4, 3}, {4, 2}, {5, 4}, {6, 4}, {7, 4}, {3, 4}, {2, 4}, {1, 4}, {0, 4}};
  EXPECT_EQ(ToSet(moves), expected);
}
