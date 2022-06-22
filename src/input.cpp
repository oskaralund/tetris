#include "input.hpp"

#include "game.hpp"
#include "render.hpp"
#include "input_states.hpp"


namespace Tetris {


Controller::Controller(Game* game, Renderer* renderer)
    : game_{game}
    , window_{renderer->window()}
    , renderer_{renderer} {
  state_ = std::make_unique<InputStatePlaying>(InputInfo{game_, renderer_});
}


bool Controller::HandleInput() {
  auto new_state = state_->Transition();
  if (new_state) {
    state_ = std::move(new_state);
  }

  SDL_Event e;

  auto pending = SDL_PollEvent(&e);
  while (pending) {
    if (e.type == SDL_QUIT) {
      return true;
    }
    state_->HandleEvent(e);
    pending = SDL_PollEvent(&e);
  }

  return false;
}


} // namespace Tetris
