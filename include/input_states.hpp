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


// Input state base class. We subclass this to specify particular states and
// how they should handle input.
class InputStateBase {
public:
  InputStateBase(InputInfo);

  virtual void HandleEvent(const SDL_Event&) = 0;
  virtual std::unique_ptr<InputStateBase> Transition() = 0;

  InputInfo info;
};


class InputStatePlaying : public InputStateBase {
public:
  using InputStateBase::InputStateBase;
  void HandleEvent(const SDL_Event&) override;
  std::unique_ptr<InputStateBase> Transition() override;
};


class InputStatePaused : public InputStateBase {
public:
  using InputStateBase::InputStateBase;
  void HandleEvent(const SDL_Event&) override;
  std::unique_ptr<InputStateBase> Transition() override;
};


class InputStateRowClear : public InputStateBase {
public:
  using InputStateBase::InputStateBase;
  void HandleEvent(const SDL_Event&) override;
  std::unique_ptr<InputStateBase> Transition() override;
};


} // namespace Tetris


#endif
