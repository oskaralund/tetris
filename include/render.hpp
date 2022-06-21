#ifndef TETRIS_RENDER_HPP_
#define TETRIS_RENDER_HPP_


#include <memory>

#include <SDL2/SDL.h>

#include "SDL_FontCache.h"
#include "game.hpp"


namespace Tetris {


class RenderState {
public:
  virtual void Render(const Game&, SDL_Renderer*) = 0;
  virtual void Enter(const Game&, SDL_Renderer*) {}
  virtual void Exit(const Game&, SDL_Renderer*) {}
  virtual std::unique_ptr<RenderState> Transition(const Game&) = 0;

  int dx() const;
  int dy() const;
  int grid_width() const;
  int grid_height() const;

private:
  const int dx_ = 25;
  const int dy_ = 25;
  const int grid_width_ = 10*dx_;
  const int grid_height_ = 24*dy_;
};


class PlayingState : public RenderState {
public:
  void Render(const Game&, SDL_Renderer*) override;
  std::unique_ptr<RenderState> Transition(const Game&) override;

  SDL_Color bg_color() const;
  SDL_Color filled_color() const;
  SDL_Color grid_color() const;

private:
  SDL_Color bg_ = {200, 200, 200, 255};
  SDL_Color filled_ = {140, 140, 140, 255};
  SDL_Color grid_ = {150, 150, 150, 255};
};


class RowClearState : public PlayingState {
public:
  RowClearState(std::vector<int> rows_cleared);

  std::unique_ptr<RenderState> Transition(const Game&) override;
  void Enter(const Game&, SDL_Renderer*) override;

private:
  Uint64 ticks_;
  Uint64 animation_milliseconds_ = 1000;
  std::vector<int> rows_cleared_;
};


class GameOverState : public PlayingState {
public:
  GameOverState();

  void Enter(const Game&, SDL_Renderer*) override;
  void Render(const Game&, SDL_Renderer*) override;
  std::unique_ptr<RenderState> Transition(const Game&) override;

private:
  void FillRow(const Game&, SDL_Renderer*, int row);
  Uint64 ticks_;
  Uint64 animation_milliseconds_ = 1000;
};


class Renderer {

public:
  Renderer(const Game&);
  ~Renderer();
  void Render();
  SDL_Window* window() const;

private:
  const Game& game_;
  const int dx_ = 25;
  const int dy_ = 25;
  const int width_ = 20*dx_;
  const int height_ = 24*dy_;

  SDL_Window* window_;
  SDL_Renderer* renderer_;
  std::unique_ptr<RenderState> state_;
  FC_Font* font_;
};


#endif


} // namespace Tetris
