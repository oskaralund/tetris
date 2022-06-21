#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "piece.hpp"
#include "game.hpp"
#include "tetris.hpp"
#include "render.hpp"
#include "input.hpp"


int main() {

  Tetris::Game tetris;
  Tetris::Renderer renderer{tetris};
  Tetris::Controller controller{&tetris, &renderer};

  // Game loop
  auto time = SDL_GetTicks();
  while (true) {
    bool quit = controller.HandleInput();
    if (quit) { break; }

    auto elapsed_time = (SDL_GetTicks()-time) / 1000.0;
    time = SDL_GetTicks();

    tetris.Step(elapsed_time);
    renderer.Render();
  }

  return 0;
}
