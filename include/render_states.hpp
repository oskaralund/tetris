#ifndef TETRIS_RENDER_STATES_HPP_
#define TETRIS_RENDER_STATES_HPP_


#include <memory>
#include <map>

#include <SDL2/SDL.h>

#include "SDL_FontCache.h"
#include "game.hpp"


namespace Tetris {


// This struct is passed to each RenderState subclass upon construction.
struct RenderInfo {
  const Game& game;
  SDL_Renderer* renderer;
  FC_Font* font;
};


// RenderState base class. We subclass this to specify particular states and
// how they should be rendered.
class RenderState {
public:
  RenderState(RenderInfo);

  virtual void Render() = 0;
  virtual void Enter() {}
  virtual void Exit() {}
  virtual std::unique_ptr<RenderState> Transition() = 0;

  int dx() const;
  int dy() const;
  int grid_width() const;
  int grid_height() const;

  RenderInfo info;

private:
  const int dx_ = 25;
  const int dy_ = 25;
  const int grid_width_ = 10*dx_;
  const int grid_height_ = 24*dy_;
  SDL_Color filled_color_ = {140, 140, 140, 255};
  SDL_Color grid_lines_color_ = {150, 150, 150, 255};
};


class PlayingState : public RenderState {
public:
  PlayingState(RenderInfo);

  void Render() override;
  std::unique_ptr<RenderState> Transition() override;

  void FillRow(int row);
  void FillBoard();
  void DrawGrid();

  SDL_Color bg_color() const;
  SDL_Color filled_color() const;
  SDL_Color grid_color() const;

private:
  void DrawCurrentPiece();
  void DrawDestination();
  void DrawNextPiece();
  void DrawBoard();
  void DrawScore();
  SDL_Color bg_color_ = {200, 200, 200, 255};
  SDL_Color filled_color_ = {140, 140, 140, 255};
  SDL_Color grid_lines_color_ = {150, 150, 150, 255};

  using ColorMap = std::map<Tetris::PieceType, SDL_Color>;
  ColorMap piece_colors_;
};


class RowClearState : public PlayingState {
public:
  RowClearState(RenderInfo, std::vector<int> rows_cleared);

  std::unique_ptr<RenderState> Transition() override;
  void Enter() override;

private:
  Uint64 ticks_;
  Uint64 animation_milliseconds_ = 1000;
  std::vector<int> rows_cleared_;
};


class GameOverState : public PlayingState {
public:
  GameOverState(RenderInfo);

  void Enter() override;
  void Render() override;
  std::unique_ptr<RenderState> Transition() override;

private:
  Uint64 ticks_;
  Uint64 animation_milliseconds_ = 1000;
};


class PauseState : public PlayingState {
public:
  using PlayingState::PlayingState;
  void Render() override;
  std::unique_ptr<RenderState> Transition() override;
};


} // namespace Tetris


#endif
