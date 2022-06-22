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

  if (quickdrop_) { dt *= 20.0; }

  time_ += dt;

  cleared_rows_ = ClearFullRows();

  if (time_ < pause_time_) { return; }

  time_ = 0.0;

  CheckGameOver();
  if (game_over_) { return; }

  CheckLevel();
  MovePieceDown();

  // Spawn new piece if we've hit the floor
  auto BelowFloor = [this] (Point p) { return p.first >= board.rows; };
  if (std::ranges::any_of(current_piece->points, BelowFloor)) {
    MovePieceUp();
    LockPieceAndSpawnNew();
  }

  // Spawn new piece if we've hit existing piece
  auto IntersectExistingPiece = [this] (Point p) {
    return board.matrix[p.first][p.second] != 0;
  };
  if (std::ranges::any_of(current_piece->points, IntersectExistingPiece)) {
    MovePieceUp();
    LockPieceAndSpawnNew();
  }
}


void Game::Left() {
  Points new_points = current_piece->points;
  for (auto& p : new_points) {
    p.second -= 1;
  }

  if (ValidPosition(new_points)) {
    current_piece->points = new_points;
  }
}


void Game::Right() {
  Points new_points = current_piece->points;
  for (auto& p : new_points) {
    p.second += 1;
  }

  if (ValidPosition(new_points)) {
    current_piece->points = new_points;
  }
}


void Game::HardDrop() {
  auto new_points = current_piece->points;
  MovePieceDown(&new_points);

  while (true) {
    auto new_points = current_piece->points;
    MovePieceDown(&new_points);
    if (ValidPosition(new_points)) {
      current_piece->points = new_points;
      continue;
    } else {
      break;
    }
  }

  time_ = pause_time_;
}


void Game::RotateCW() {
  auto points = current_piece->points;
  auto rotation = current_piece->rotation;
  auto type = current_piece->type;


  // Try basic rotation
  current_piece->RotateCW();

  if (!ValidPosition(current_piece->points)) {
    current_piece->points = points;
    current_piece->rotation = rotation;
  } else { return; }


  // Try wallkicks
  std::vector<std::pair<int, int>> kicks;
  if (type == PieceType::I) {
    kicks = I_kicks_[rotation];
  } else {
    kicks = JLTSZ_kicks_[rotation];
  }

  for (const auto& kick : kicks) {
    current_piece->RotateCW();
    current_piece->Translate(kick.first, kick.second);

    if (!ValidPosition(current_piece->points)) {
      current_piece->points = points;
      current_piece->rotation = rotation;
    } else { return; }
  }
}


bool Game::GameOver() const {
  return game_over_;
}


void Game::Restart() {
  game_over_ = false;
  time_ = 0.0;
  level_progression_ = 0;
  level_ = 1;
  score_ = 0;
  current_piece = GetRandomPiece();
  next_piece = GetRandomPiece();

  board.Clear();
}


Points Game::GetDestination() const {
  auto destination = current_piece->points;

  while (ValidPosition(destination)) {
    for (auto& p : destination) {
      p.first += 1;
    }
  }

  for (auto& p : destination) {
    p.first -= 1;
  }

  return destination;
}


void Game::MovePieceDown() {
  for (auto& p : current_piece->points) {
    p.first += 1;
  }
}


void Game::MovePieceUp() {
  for (auto& p : current_piece->points) {
    p.first -= 1;
  }
}


void Game::LockPieceAndSpawnNew() {
  if (!ValidPosition(current_piece->points)) {
    game_over_ = true;
    return;
  }

  for (const auto& p : current_piece->points) {
    board.matrix[p.first][p.second] = 1;
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


ClearedRows Game::ClearFullRows() {
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


bool Game::ValidPosition(Points points) const {
  for (const auto& p : points) {
    if (p.first < 0 ||
        p.first > board.rows-1 ||
        p.second < 0 ||
        p.second > board.cols-1) {
      return false;
    }
  }

  for (const auto& p : points) {
    if (board.matrix[p.first][p.second] != 0) {
      return false;
    }
  }

  return true;
}


void Game::CheckGameOver() {
  auto InRowZero = [this] (Point p) { return p.first == 0; };
  auto IntersectExistingPiece = [this] (Point p) {
    return board.matrix[p.first][p.second] != 0;
  };

  if (std::ranges::any_of(current_piece->points, InRowZero) &&
      std::ranges::any_of(current_piece->points, IntersectExistingPiece)) {
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


void Game::MovePieceDown(Points* points) const {
  for (auto& p : *points) { p.first += 1; }
}

void Game::MovePieceUp(Points* points) const {
  for (auto& p : *points) { p.first -= 1; }
}


} // namespace Tetris
