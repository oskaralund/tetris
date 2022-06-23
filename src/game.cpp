#include "game.hpp"

#include <algorithm>
#include <cstdlib>
#include <chrono>

#include "piece.hpp"

namespace Tetris {


Game::Game() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  rng_.seed(seed);

  current_piece = GetRandomPiece();
  next_piece = GetRandomPiece();
  board.Clear();

  JLTSZ_kicks_ = {{{0, -1}, {-1, -1}, { 2, 0}, { 2, -1}},
                  {{0,  1}, { 1,  1}, {-2, 0}, {-2,  1}},
                  {{0,  1}, {-1,  1}, { 2, 0}, { 2,  1}},
                  {{0, -1}, { 1, -1}, {-2, 0}, {-2, -1}}};

  I_kicks_ = {{{0, -2}, { 0,  1}, { 1, -2}, {-2,  1}},
              {{0, -1}, { 0,  2}, {-2, -1}, { 1,  2}},
              {{0,  2}, { 0, -1}, {-1,  2}, { 2, -1}},
              {{0,  1}, { 0, -2}, { 2,  1}, {-1, -2}}};
}


void Game::QuickDrop(bool q) {
  quickdrop_ = q;
}

uint64_t Game::score() const { return score_; }
uint64_t Game::level() const { return level_; }

std::vector<int> Game::GetClearedRows() const {
  return cleared_rows_;
}


void Game::Step(double dt) {

  if (paused_) { return; }

  if (quickdrop_) { dt *= 20.0; }

  time_ += dt;

  cleared_rows_ = ClearFullRows();

  if (time_ < pause_time_) { return; }

  time_ = 0.0;

  CheckGameOver();
  if (game_over_) { return; }

  CheckLevel();
  *current_piece += {1, 0};

  // Spawn new piece if we've hit the floor
  auto BelowFloor = [this] (Point p) { return p.row >= board.rows; };
  if (std::ranges::any_of(*current_piece, BelowFloor)) {
    *current_piece += {-1, 0};
    LockPieceAndSpawnNew();
  }

  // Spawn new piece if we've hit existing piece
  auto IntersectExistingPiece = [this] (Point p) {
    return board.matrix[p.row][p.col] != 0;
  };
  if (std::ranges::any_of(*current_piece, IntersectExistingPiece)) {
    *current_piece += {-1, 0};
    LockPieceAndSpawnNew();
  }
}


void Game::MovePieceLeft() {
  *current_piece += {0, -1};
  if (!ValidPosition(*current_piece)) {
    *current_piece += {0, 1};
  }
}


void Game::MovePieceRight() {
  *current_piece += {0, 1};
  if (!ValidPosition(*current_piece)) {
    *current_piece += {0, -1};
  }
}


void Game::HardDrop() {
  while (true) {
    *current_piece += {1, 0};

    if (!ValidPosition(*current_piece)) {
      *current_piece += {-1, 0};
      break;
    }
  }

  time_ = pause_time_;
}


void Game::RotatePiece() {
  // Try basic rotation
  current_piece->RotateCW();

  if (!ValidPosition(*current_piece)) {
    current_piece->RotateCCW();
  } else { return; }


  // Try wallkicks
  const auto type = current_piece->type;
  const auto rotation = current_piece->rotation;
  std::vector<std::pair<int, int>> kicks;
  if (type == PieceType::I) {
    kicks = I_kicks_[rotation];
  } else {
    kicks = JLTSZ_kicks_[rotation];
  }

  for (const auto& kick : kicks) {
    current_piece->RotateCW();
    *current_piece += {kick.first, kick.second};

    if (!ValidPosition(*current_piece)) {
      *current_piece += {-kick.first, -kick.second};
      current_piece->RotateCCW();
    } else { return; }
  }
}


bool Game::gameover() const {
  return game_over_;
}


bool Game::paused() const {
  return paused_;
}


void Game::Restart() {
  game_over_ = false;
  time_ = 0.0;
  level_progression_ = 0;
  level_ = 1;
  score_ = 0;
  paused_ = false;
  pause_time_ = 1.0;
  current_piece = GetRandomPiece();
  next_piece = GetRandomPiece();

  board.Clear();
}


void Game::TogglePause() {
  paused_ = !paused_;
}


Points Game::GetDestination() const {
  Points destination = *current_piece;

  while (ValidPosition(destination)) {
    destination += {1, 0};
  }

  destination += {-1, 0};

  return destination;
}



void Game::LockPieceAndSpawnNew() {
  if (!ValidPosition(*current_piece)) {
    game_over_ = true;
    return;
  }

  for (const auto& p : *current_piece) {
    board.matrix[p.row][p.col] = 1;
  }

  current_piece = std::move(next_piece);
  next_piece = GetRandomPiece();
}


bool Game::RowIsFull(int row) const {
  for (int j = 0; j < board.cols; ++j) {
    if (board.matrix[row][j] == 0) {
      return false;
    }
  }

  return true;
}


void Game::DropRows(int row) {
  if (row >= board.rows || row < 0) { return; }

  for (int i = row; i >= 0; --i) {
    for (int j = 0; j < board.cols; ++j) {
      board.matrix[i+1][j] = board.matrix[i][j];
    }
  }

  for (int j = 0; j < board.cols; ++j) {
    board.matrix[0][j] = 0;
  }
}


Game::ClearedRows Game::ClearFullRows() {
  ClearedRows cleared_rows;
  for (int i = 0; i < board.rows; ++i) {
    if (RowIsFull(i)) {
      cleared_rows.push_back(i);
      DropRows(i-1);
    }
  }

  switch (cleared_rows.size()) {
    case 1:
      score_ += 100*level_;
      break;
    case 2:
      score_ += 300*level_;
      break;
    case 3:
      score_ += 500*level_;
      break;
    case 4:
      score_ += 800*level_;
      break;
  }

  level_progression_ += cleared_rows.size();

  return cleared_rows;
}


bool Game::ValidPosition(const Points& points) const {
  for (const auto& p : points) {
    if (p.row < 0 ||
        p.row > board.rows-1 ||
        p.col < 0 ||
        p.col > board.cols-1) {
      return false;
    }
  }

  for (const auto& p : points) {
    if (board.matrix[p.row][p.col] != 0) {
      return false;
    }
  }

  return true;
}


void Game::CheckGameOver() {
  auto InRowZero = [this] (Point p) { return p.row == 0; };
  auto IntersectExistingPiece = [this] (Point p) {
    return board.matrix[p.row][p.col] != 0;
  };

  if (std::ranges::any_of(*current_piece, InRowZero) &&
      std::ranges::any_of(*current_piece, IntersectExistingPiece)) {
    game_over_ = true;
  }
}


void Game::CheckLevel() {
  if (level_progression_ >= 10) {
    level_progression_ = 0;
    pause_time_ *= 0.9;
    level_ += 1;
  }
}


Game::Piece_ptr Game::GetRandomPiece() {
  std::uniform_int_distribution<int> dist{0,6};
  auto n = dist(rng_);

  switch (n) {
    case 0:
      return std::make_unique<IPiece>();
    case 1:
      return std::make_unique<JPiece>();
    case 2:
      return std::make_unique<LPiece>();
    case 3:
      return std::make_unique<OPiece>();
    case 4:
      return std::make_unique<SPiece>();
    case 5:
      return std::make_unique<TPiece>();
    case 6:
      return std::make_unique<ZPiece>();
  }

  return std::make_unique<LPiece>();
}


} // namespace Tetris
