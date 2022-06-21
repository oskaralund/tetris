#ifndef TETRIS_INPUT_HPP_
#define TETRIS_INPUT_HPP_


#include <SDL2/SDL.h>


namespace Tetris {


class Game;
class Renderer;


class Controller {
public:
  Controller(Game*, Renderer*);
  bool HandleInput(); // Return true if quitting

private:
  void HandleEvent(const SDL_Event&);
  Game* game_;
  SDL_Window* window_;
  Renderer* renderer_;
};


} // namespace Tetris


#endif
