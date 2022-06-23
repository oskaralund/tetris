#ifndef TETRIS_INPUT_HPP_
#define TETRIS_INPUT_HPP_

#include <memory>

#include <SDL2/SDL.h>

#include "input_states.hpp"


namespace Tetris {


class Game;
class Renderer;


class Controller {
public:
  Controller(Game*, Renderer*);
  bool HandleInput(); // Return true if quitting

private:
  Game* game_;
  SDL_Window* window_;
  Renderer* renderer_;
  std::unique_ptr<InputStateBase> state_;
};


} // namespace Tetris


#endif
