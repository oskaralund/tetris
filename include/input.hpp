#ifndef TETRIS_INPUT_HPP_
#define TETRIS_INPUT_HPP_

#include <memory>

#include <SDL2/SDL.h>

#include "input_states.hpp"


namespace Tetris {


class Game;
class Renderer;


// This class handles input to a Game instance. It uses a finite state machine
// to handle input differently depending on the state of the game and renderer.
// See input_states.hpp.
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
