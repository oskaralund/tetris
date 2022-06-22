#ifndef TETRIS_GAME_HPP_
#define TETRIS_GAME_HPP_

#include <memory>
#include <random>

#include "board.hpp"
#include "piece.hpp"


namespace Tetris {


class Piece;

using ClearedRows = std::vector<int>;

class Game {
public:
  using Piece_ptr = std::unique_ptr<Piece>;

  Game();
  void Step(double dt); // Call in game loop to move game forward
  void MovePieceLeft();
  void MovePieceRight();
  void HardDrop(); // Immediately drop piece to floor
  void QuickDrop(bool); // Increase drop speed
  void RotatePiece();
  void Restart();

  // Get indices of recently cleared rows
  std::vector<int> GetClearedRows() const;

  // Get points corresponding to where the current piece will end up
  Points GetDestination() const;

  uint64_t score() const;
  uint64_t level() const;
  bool gameover() const; 

  Board board;
  Piece_ptr current_piece;
  Piece_ptr next_piece;

private:
  using WallKickTable = std::vector<std::vector<std::pair<int, int>>>;

  void LockPieceAndSpawnNew();
  void DropRows(int);
  ClearedRows ClearFullRows();
  void MovePieceDown();
  void MovePieceUp();
  void CheckGameOver();
  void CheckLevel();
  bool RowIsFull(int) const;
  bool ValidPosition(Points) const;
  Piece_ptr GetRandomPiece();

  double time_ = 0.0;
  double pause_time_ = 1.0;
  bool game_over_ = false;
  bool quickdrop_ = false;
  uint64_t level_ = 1;
  uint64_t score_ = 0;
  uint8_t level_progression_ = 0;
  std::vector<int> cleared_rows_;
  std::default_random_engine rng_;

  WallKickTable JLTSZ_kicks_;
  WallKickTable I_kicks_;
};


} // namespace Tetris


#endif
