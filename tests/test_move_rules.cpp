#include <gtest/gtest.h>

#include <set>

#include "chess/Board.h"
#include "chess/MoveRules.h"

namespace {
std::set<Position> ToSet(const std::vector<Position>& moves) {
  return std::set<Position>(moves.begin(), moves.end());
}

std::set<Position> QueenExpectedMoves(Position from) {
  std::set<Position> expected;
  for (int i = 0; i < 8; ++i) {
    if (i != from.row) {
      expected.insert({i, from.col});
    }
    if (i != from.col) {
      expected.insert({from.row, i});
    }
  }

  for (int dr : {-1, 1}) {
    for (int dc : {-1, 1}) {
      int row = from.row + dr;
      int col = from.col + dc;
      while (row >= 0 && row < 8 && col >= 0 && col < 8) {
        expected.insert({row, col});
        row += dr;
        col += dc;
      }
    }
  }
  return expected;
}
}  // namespace

TEST(MoveRules, KnightJumpsOverPieces) {
  Board board = Board::StandardSetup();
  MoveRules rules;
  auto moves = rules.pseudoLegalMoves(board, {7, 1});

  std::set<Position> expected = {{5, 0}, {5, 2}};
  EXPECT_EQ(ToSet(moves), expected);
}

TEST(MoveRules, BishopBlockedAtStart) {
  Board board = Board::StandardSetup();
  MoveRules rules;
  auto moves = rules.pseudoLegalMoves(board, {7, 2});
  EXPECT_TRUE(moves.empty());
}

TEST(MoveRules, PawnInitialMoves) {
  Board board = Board::StandardSetup();
  MoveRules rules;
  auto moves = rules.pseudoLegalMoves(board, {6, 0});

  std::set<Position> expected = {{5, 0}, {4, 0}};
  EXPECT_EQ(ToSet(moves), expected);
}

TEST(MoveRules, PawnCapturesDiagonal) {
  Board board;
  board.placePiece({4, 4}, {PieceType::Pawn, Color::White});
  board.placePiece({3, 3}, {PieceType::Knight, Color::Black});
  board.placePiece({3, 5}, {PieceType::Bishop, Color::Black});

  MoveRules rules;
  auto moves = rules.pseudoLegalMoves(board, {4, 4});
  std::set<Position> expected = {{3, 4}, {3, 3}, {3, 5}};
  EXPECT_EQ(ToSet(moves), expected);
}

TEST(MoveRules, PawnEdgeCapture) {
  Board board;
  board.placePiece({4, 0}, {PieceType::Pawn, Color::White});
  board.placePiece({3, 1}, {PieceType::Knight, Color::Black});

  MoveRules rules;
  auto moves = rules.pseudoLegalMoves(board, {4, 0});
  std::set<Position> expected = {{3, 0}, {3, 1}};
  EXPECT_EQ(ToSet(moves), expected);
}

TEST(MoveRules, RookStopsAtBlockingPiece) {
  Board board;
  board.placePiece({4, 4}, {PieceType::Rook, Color::White});
  board.placePiece({4, 6}, {PieceType::Knight, Color::White});
  board.placePiece({4, 2}, {PieceType::Knight, Color::Black});

  MoveRules rules;
  auto moves = rules.pseudoLegalMoves(board, {4, 4});
  std::set<Position> expected = {
      {4, 5}, {4, 3}, {4, 2}, {5, 4}, {6, 4}, {7, 4}, {3, 4}, {2, 4}, {1, 4}, {0, 4}};
  EXPECT_EQ(ToSet(moves), expected);
}

TEST(MoveRules, KingMovesOneSquare) {
  Board board;
  board.placePiece({4, 4}, {PieceType::King, Color::White});

  MoveRules rules;
  auto moves = rules.pseudoLegalMoves(board, {4, 4});
  std::set<Position> expected = {
      {3, 3}, {3, 4}, {3, 5},
      {4, 3},         {4, 5},
      {5, 3}, {5, 4}, {5, 5}};
  EXPECT_EQ(ToSet(moves), expected);
}

TEST(MoveRules, QueenMovesAlongLines) {
  Board board;
  board.placePiece({4, 4}, {PieceType::Queen, Color::White});

  MoveRules rules;
  auto moves = rules.pseudoLegalMoves(board, {4, 4});
  EXPECT_EQ(ToSet(moves), QueenExpectedMoves({4, 4}));
}

TEST(MoveRules, EmptyOrOutOfRangeReturnsEmpty) {
  Board board = Board::StandardSetup();
  MoveRules rules;
  EXPECT_TRUE(rules.pseudoLegalMoves(board, {3, 3}).empty());
  EXPECT_TRUE(rules.pseudoLegalMoves(board, {-1, 0}).empty());
}
