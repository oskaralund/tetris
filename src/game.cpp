#include "game.hpp"

#include <algorithm>
#include <cstdlib>

#include "piece.hpp"

namespace Tetris {


Game::Game() {
  current_piece = GetRandomPiece();
  next_piece = GetRandomPiece();
  board.Clear();
}


ClearedRows Game::Step(double dt) {

  time_ += dt;

  if (time_ < pause_time_) { return {}; }

  time_ = 0.0;

  CheckGameOver();
  if (game_over_) { return {}; }

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

  return ClearFullRows();
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


void Game::Down() {
  auto new_points = current_piece->points;
  MovePieceDown(&new_points);

  if (!ValidPosition(new_points)) {
    time_ = pause_time_;
    return;
  }

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
  time_ = 0.0;
}


void Game::RotateCW() {
  auto prev_points = current_piece->points;
  auto prev_rotation = current_piece->rotation;

  current_piece->RotateCW();

  if (!ValidPosition(current_piece->points)) {
    current_piece->points = prev_points;
    current_piece->rotation = prev_rotation;
  }
}


bool Game::GameOver() {
  return game_over_;
}


void Game::Restart() {
  game_over_ = false;
  time_ = 0.0;
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


Game::Piece_ptr Game::GetRandomPiece() {
  auto n = rand() % 7;

  switch (n) {
    case 0:
      return std::make_unique<IPiece>(&board);
    case 1:
      return std::make_unique<LPiece>(&board);
    case 2:
      return std::make_unique<JPiece>(&board);
    case 3:
      return std::make_unique<OPiece>(&board);
    case 4:
      return std::make_unique<SPiece>(&board);
    case 5:
      return std::make_unique<TPiece>(&board);
    case 6:
      return std::make_unique<ZPiece>(&board);
  }

  return std::make_unique<LPiece>(&board);
}


void Game::MovePieceDown(Points* points) const {
  for (auto& p : *points) { p.first += 1; }
}

void Game::MovePieceUp(Points* points) const {
  for (auto& p : *points) { p.first -= 1; }
}


} // namespace Tetris
