#ifndef TETRIS_RENDER_HPP_
#define TETRIS_RENDER_HPP_


#include <memory>
#include <map>

#include <SDL2/SDL.h>

#include "SDL_FontCache.h"
#include "game.hpp"
#include "render_states.hpp"


namespace Tetris {


// This class renders an instance of Game. It uses a finite state machine
// which changes what is rendered depending on the state of the game (e.g. if
// rows are being cleared, if the game is over, etc). See render_states.hpp
// for the different states and how they are rendered.
class Renderer {
public:
  Renderer(const Game&);
  ~Renderer();
  void Render();
  SDL_Window* window() const;
  RenderStateName state() const;

private:
  const Game& game_;
  const int dx_ = 25;
  const int dy_ = 25;
  const int width_ = 18*dx_;
  const int height_ = 24*dy_;

  SDL_Window* window_;
  SDL_Renderer* renderer_;
  std::unique_ptr<RenderStateBase> state_;
  FC_Font* font_;
};


#endif


} // namespace Tetris
