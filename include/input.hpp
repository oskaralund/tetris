#ifndef TETRIS_INPUT_HPP_
#define TETRIS_INPUT_HPP_

#include <memory>

#include <SDL2/SDL.h>


namespace Tetris {


class Game;
class Renderer;


// This class handles input to a Game instance.
class Controller {
public:
  Controller(Game*);
  bool HandleInput(); // Return true if quitting

private:
  void Playing(const SDL_Event&);
  void RowClear(const SDL_Event&);
  void Paused(const SDL_Event&);

  Game* game_;
};


} // namespace Tetris


#endif
