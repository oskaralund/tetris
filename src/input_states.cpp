#include "input_states.hpp"

#include "render.hpp"


namespace Tetris {


InputStateBase::InputStateBase(InputInfo info) : info{info} {}


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


std::unique_ptr<InputStateBase> InputStatePlaying::Transition() {
  switch (info.renderer->state()) {
    case RenderStateName::PAUSED:
      return std::make_unique<InputStatePaused>(info);
    case RenderStateName::ROWCLEAR:
      return std::make_unique<InputStateRowClear>(info);
    default:
      return nullptr;
  }
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


std::unique_ptr<InputStateBase> InputStatePaused::Transition() {
  if (!info.game->paused()) {
    return std::make_unique<InputStatePlaying>(info);
  }

  return nullptr;
}


// InputStateRowClear
void InputStateRowClear::HandleEvent(const SDL_Event& e) {
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

  if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {
      case SDLK_DOWN:
        info.game->QuickDrop(false);
        break;
    }
  }
}


std::unique_ptr<InputStateBase> InputStateRowClear::Transition() {
  switch (info.renderer->state()) {
    case RenderStateName::PLAYING:
      return std::make_unique<InputStatePlaying>(info);
    case RenderStateName::PAUSED:
      return std::make_unique<InputStatePaused>(info);
    default:
      return nullptr;
  }
}


} // namespace Tetris
