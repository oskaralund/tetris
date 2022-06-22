#include "input.hpp"

#include "game.hpp"
#include "render.hpp"


namespace Tetris {


Controller::Controller(Game* game, Renderer* renderer)
    : game_{game}
    , window_{renderer->window()}
    , renderer_{renderer} {}


bool Controller::HandleInput() {
  SDL_Event e;

  auto pending = SDL_PollEvent(&e);
  while (pending) {
    if (e.type == SDL_QUIT) {
      return true;
    }
    HandleEvent(e);
    pending = SDL_PollEvent(&e);
  }

  return false;
}


void Controller::HandleEvent(const SDL_Event& e) {
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_LEFT:
        game_->MovePieceLeft();
        break;
      case SDLK_RIGHT:
        game_->MovePieceRight();
        break;
      case SDLK_DOWN:
        game_->QuickDrop(true);
        break;
      case SDLK_UP:
        game_->RotatePiece();
        break;
      case SDLK_SPACE:
        game_->HardDrop();
        break;
      case SDLK_r:
        game_->Restart();
        break;
      case SDLK_p:
        game_->TogglePause();
        break;
    }
  }

  if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {
      case SDLK_DOWN:
        game_->QuickDrop(false);
        break;
    }
  }
}

} // namespace Tetris
