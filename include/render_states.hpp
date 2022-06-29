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


// Render state base class. We subclass this to specify particular states and
// how they should be rendered.
class RenderStateBase {
public:
  RenderStateBase(RenderInfo);

  virtual void Render() = 0;
  virtual std::unique_ptr<RenderStateBase> Transition() = 0;

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
};


class RenderStatePlaying : public RenderStateBase {
public:
  RenderStatePlaying(RenderInfo);

  void Render() override;
  std::unique_ptr<RenderStateBase> Transition() override;

  void FillRow(int row);
  void FillBoard();

  SDL_Color bg_color() const;
  SDL_Color filled_color() const;
  SDL_Color grid_color() const;

private:
  void DrawGrid();
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


class RenderStateRowClear : public RenderStatePlaying {
public:
  RenderStateRowClear(RenderInfo);

  void Render() override;
  std::unique_ptr<RenderStateBase> Transition() override;

private:
  Uint64 ticks_;
  std::vector<int> rows_cleared_;
};


class RenderStateGameOver : public RenderStatePlaying {
public:
  RenderStateGameOver(RenderInfo);

  void Render() override;
  std::unique_ptr<RenderStateBase> Transition() override;

private:
  Uint64 ticks_;
  Uint64 animation_milliseconds_ = 1000;
};


class RenderStatePaused : public RenderStatePlaying {
public:
  RenderStatePaused(RenderInfo);

  void Render() override;
  std::unique_ptr<RenderStateBase> Transition() override;
};


} // namespace Tetris


#endif
