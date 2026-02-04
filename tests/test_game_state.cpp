#include <gtest/gtest.h>

#include <utility>

#include "chess/GameState.h"
#include "chess/CheckDetector.h"
#include "chess/ICheckDetector.h"
#include "chess/IMoveRules.h"
#include "chess/MoveRules.h"

namespace {
class FakeMoveRules : public IMoveRules {
 public:
  explicit FakeMoveRules(std::vector<Position> moves) : moves_(std::move(moves)) {}

  std::vector<Position> pseudoLegalMoves(const Board&, Position) const override {
    return moves_;
  }

 private:
  std::vector<Position> moves_;
};

class FakeCheckDetector : public ICheckDetector {
 public:
  bool inCheck(const Board&, Color) const override { return false; }
};
}  // namespace

TEST(GameState, TurnAlternatesOnValidMove) {
  auto rules = std::make_unique<MoveRules>();
  GameState game = GameState::Standard(std::move(rules), std::make_unique<FakeCheckDetector>());

  EXPECT_EQ(game.turn(), Color::White);
  EXPECT_TRUE(game.tryMove({6, 0}, {5, 0}));
  EXPECT_EQ(game.turn(), Color::Black);

  EXPECT_FALSE(game.tryMove({6, 1}, {5, 1}));
  EXPECT_TRUE(game.tryMove({1, 0}, {2, 0}));
  EXPECT_EQ(game.turn(), Color::White);
}

TEST(GameState, RejectsMoveToSameColorPiece) {
  Board board;
  board.placePiece({4, 4}, {PieceType::Rook, Color::White});
  board.placePiece({4, 6}, {PieceType::Knight, Color::White});

  GameState game(board, Color::White, std::make_unique<MoveRules>(),
                 std::make_unique<FakeCheckDetector>());

  EXPECT_FALSE(game.tryMove({4, 4}, {4, 6}));
}

TEST(GameState, EnforcesPathBlockingForSlidingPieces) {
  Board board;
  board.placePiece({4, 4}, {PieceType::Rook, Color::White});
  board.placePiece({4, 5}, {PieceType::Pawn, Color::White});

  GameState game(board, Color::White, std::make_unique<MoveRules>(),
                 std::make_unique<FakeCheckDetector>());

  EXPECT_FALSE(game.tryMove({4, 4}, {4, 7}));
}

TEST(GameState, AllowsCaptureOfOpponentPiece) {
  Board board;
  board.placePiece({4, 4}, {PieceType::Rook, Color::White});
  board.placePiece({4, 2}, {PieceType::Bishop, Color::Black});

  GameState game(board, Color::White, std::make_unique<MoveRules>(),
                 std::make_unique<FakeCheckDetector>());

  EXPECT_TRUE(game.tryMove({4, 4}, {4, 2}));
  const Piece* piece = game.board().pieceAt({4, 2});
  ASSERT_NE(piece, nullptr);
  EXPECT_EQ(piece->type, PieceType::Rook);
  EXPECT_EQ(piece->color, Color::White);
}

TEST(GameState, RejectsMoveFromEmptySquare) {
  GameState game = GameState::Standard(std::make_unique<MoveRules>(),
                                       std::make_unique<FakeCheckDetector>());
  EXPECT_FALSE(game.tryMove({3, 3}, {3, 4}));
}

TEST(GameState, UsesMoveRulesInterface) {
  Board board;
  board.placePiece({4, 4}, {PieceType::Rook, Color::White});

  GameState game(board, Color::White,
                 std::make_unique<FakeMoveRules>(std::vector<Position>{{4, 7}}),
                 std::make_unique<FakeCheckDetector>());

  EXPECT_TRUE(game.tryMove({4, 4}, {4, 7}));
  const Piece* piece = game.board().pieceAt({4, 7});
  ASSERT_NE(piece, nullptr);
  EXPECT_EQ(piece->type, PieceType::Rook);
}

TEST(GameState, StatusOngoingWhenLegalMoveExists) {
  Board board;
  board.placePiece({7, 4}, {PieceType::King, Color::White});
  board.placePiece({0, 4}, {PieceType::King, Color::Black});

  GameState game(board, Color::White,
                 std::make_unique<MoveRules>(),
                 std::make_unique<CheckDetector>());

  EXPECT_EQ(game.status(), GameStatus::Ongoing);
}

TEST(GameState, StatusNoLegalMovesInStalemateLikePosition) {
  Board board;
  board.placePiece({7, 7}, {PieceType::King, Color::White});
  board.placePiece({5, 6}, {PieceType::King, Color::Black});
  board.placePiece({6, 5}, {PieceType::Queen, Color::Black});

  GameState game(board, Color::White,
                 std::make_unique<MoveRules>(),
                 std::make_unique<CheckDetector>());

  EXPECT_EQ(game.status(), GameStatus::NoLegalMoves);
}
