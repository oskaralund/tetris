#ifndef TETRIS_INPUT_STATES_HPP_
#define TETRIS_INPUT_STATES_HPP_


#include <memory>

#include <SDL2/SDL.h>

#include "game.hpp"


namespace Tetris {


class Renderer;


// This struct is passed to each InputState subclass upon construction.
struct InputInfo {
  Game* game;
  Renderer* renderer;
};


// InputState base class. We subclass this to specify particular states and
// how they should handle input.
class InputState {
public:
  InputState(InputInfo);

  virtual void HandleEvent(const SDL_Event&) = 0;
  virtual std::unique_ptr<InputState> Transition() = 0;

  InputInfo info;
};


class InputStatePlaying : public InputState {
public:
  using InputState::InputState;
  void HandleEvent(const SDL_Event&) override;
  std::unique_ptr<InputState> Transition() override;
};


class InputStatePaused : public InputState {
public:
  using InputState::InputState;
  void HandleEvent(const SDL_Event&) override;
  std::unique_ptr<InputState> Transition() override;
};


} // namespace Tetris


#endif
