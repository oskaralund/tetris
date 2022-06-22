#include "input_states.hpp"

#include "render.hpp"


namespace Tetris {


InputState::InputState(InputInfo info) : info{info} {}


// InputStatePlaying
void InputStatePlaying::HandleEvent(const SDL_Event& e) {
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_LEFT:
        info.game->MovePieceLeft();
        break;
      case SDLK_RIGHT:
        info.game->MovePieceRight();
        break;
      case SDLK_DOWN:
        info.game->QuickDrop(true);
        break;
      case SDLK_UP:
        info.game->RotatePiece();
        break;
      case SDLK_SPACE:
        info.game->HardDrop();
        break;
      case SDLK_r:
        info.game->Restart();
        break;
      case SDLK_p:
        info.game->TogglePause();
        break;
    }
  }

  if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {
      case SDLK_DOWN:
        info.game->QuickDrop(false);
        break;
    }
  }
}


std::unique_ptr<InputState> InputStatePlaying::Transition() {
  if (info.game->paused()) {
    return std::make_unique<InputStatePaused>(info);
  }

  return nullptr;
}


// InputStatePaused
void InputStatePaused::HandleEvent(const SDL_Event& e) {
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_r:
        info.game->Restart();
        break;
      case SDLK_p:
        info.game->TogglePause();
        break;
    }
  }
}


std::unique_ptr<InputState> InputStatePaused::Transition() {
  if (!info.game->paused()) {
    return std::make_unique<InputStatePlaying>(info);
  }

  return nullptr;
}


} // namespace Tetris
