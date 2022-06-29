#include "input.hpp"

#include "game.hpp"
#include "render.hpp"


namespace Tetris {


Controller::Controller(Game* game) : game_{game} {}


bool Controller::HandleInput() {
  SDL_Event e;

  auto pending = SDL_PollEvent(&e);
  while (pending) {
    if (e.type == SDL_QUIT) {
      return true;
    }

    switch (game_->state()) {
      case Game::State::PLAYING:
        Playing(e);
        break;
      case Game::State::PAUSED:
        Paused(e);
        break;
      case Game::State::GAMEOVER:
        Paused(e);
        break;
      case Game::State::ROWCLEAR:
        RowClear(e);
        break;
    }
    pending = SDL_PollEvent(&e);
  }

  return false;
}


void Controller::Playing(const SDL_Event& e) {
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
      case SDLK_ESCAPE:
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


void Controller::Paused(const SDL_Event& e) {
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_r:
        game_->Restart();
        break;
      case SDLK_ESCAPE:
        game_->TogglePause();
        break;
    }
  }
}


void Controller::RowClear(const SDL_Event& e) {
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_r:
        game_->Restart();
        break;
      case SDLK_ESCAPE:
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
