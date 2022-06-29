#ifndef TETRIS_GAME_HPP_
#define TETRIS_GAME_HPP_

#include <memory>
#include <random>
#include <functional>

#include "board.hpp"
#include "piece.hpp"


namespace Tetris {


class Game {
public:
  using Piece_ptr = std::unique_ptr<Piece>;
  using RowIndices = std::vector<int>;

  enum class State {
    PLAYING, PAUSED, ROWCLEAR, GAMEOVER
  };

  Game();
  void Step(double dt); // Call in game loop to move game forward
  void MovePieceLeft();
  void MovePieceRight();
  void HardDrop(); // Immediately drop piece to floor
  void QuickDrop(bool); // Increase drop speed
  void RotatePiece();
  void Restart();
  void TogglePause();
  RowIndices GetFullRows() const;

  // Get points corresponding to where the current piece will end up
  Points GetDestination() const;

  uint64_t score() const;
  uint64_t level() const;
  State state() const;

  Board board;
  Piece_ptr current_piece;
  Piece_ptr next_piece;

private:
  using WallKickTable = std::vector<std::vector<std::pair<int, int>>>;
  using StateStack = std::vector<std::function<void (Game*, double)>>;

  void LockPieceAndSpawnNew();
  void DropRows(int);
  void ClearFullRows();
  void CheckGameOver();
  void CheckLevel();
  bool RowIsFull(int) const;
  bool ValidPosition(const Points&) const;
  Piece_ptr GetRandomPiece();

  // States
  void PlayingStep(double dt);
  void RowClearStep(double dt);
  void PausedStep(double dt);
  void GameOverStep(double dt);

  double time_ = 0.0;
  double pause_time_ = 1.0;
  bool game_over_ = false;
  bool paused_ = false;
  uint64_t level_ = 1;
  uint64_t score_ = 0;
  uint8_t level_progression_ = 0;
  std::default_random_engine rng_;
  WallKickTable JLTSZ_kicks_;
  WallKickTable I_kicks_;
  StateStack states_;
  State state_;
};


} // namespace Tetris


#endif
