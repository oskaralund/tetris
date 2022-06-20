#pragma once

#include <memory>

#include "board.hpp"
#include "tetris.hpp"


namespace Tetris {


class Piece;

using ClearedRows = std::vector<int>;

class Game {
public:
  using Piece_ptr = std::unique_ptr<Piece>;

  Game();
  ClearedRows Step(double dt);
  void Left();
  void Right();
  void Down();
  void RotateCW();
  bool GameOver();
  void Restart();
  Points GetDestination() const;

  Board board;
  Piece_ptr current_piece;
  Piece_ptr next_piece;

private:

  void LockPieceAndSpawnNew();
  void DropRows(int);
  ClearedRows ClearFullRows();
  void MovePieceDown();
  void MovePieceDown(Points*) const;
  void MovePieceUp();
  void MovePieceUp(Points*) const;
  void CheckGameOver();
  bool RowIsFull(int) const;
  bool ValidPosition(Points) const;
  Piece_ptr GetRandomPiece();

  double time_ = 0.0;
  double pause_time_ = 1.0;
  bool game_over_ = false;
};


} // namespace Tetris
