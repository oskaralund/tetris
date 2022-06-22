#include "render.hpp"

#include <map>
#include <string>

#include "piece.hpp"


namespace Tetris {


Renderer::Renderer(const Game& g) : game_{g} {

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
  }

  // Initialize SDL_ttf
  if (TTF_Init() == -1) {
    printf("TTF_Init: %s\n", TTF_GetError());
  }

  // Create window
  window_ = SDL_CreateWindow(
      "Tetris",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      width_,
      height_,
      SDL_WINDOW_SHOWN);

  if (window_ == nullptr) {
    printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
  }

  // Create renderer
  const int driver = -1;
  renderer_ = SDL_CreateRenderer(window_, driver, SDL_RENDERER_ACCELERATED);

  if (renderer_ == nullptr) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
  }

  SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

  // Initialize SDL_ttf
  font_ = FC_CreateFont();
  FC_LoadFont(
      font_,
      renderer_,
      "FreeSans.ttf",
      20, 
      FC_MakeColor(0, 0, 0, 255),
      TTF_STYLE_NORMAL);

  // Set playing state
  state_ = std::make_unique<PlayingState>(RenderInfo{game_, renderer_, font_});
}


Renderer::~Renderer() {
  FC_FreeFont(font_);
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  TTF_Quit();
  SDL_Quit();
}


void Renderer::Render() {
  auto new_state = state_->Transition();

  if (new_state) {
    state_ = std::move(new_state);
    state_->Enter();
  }

  state_->Render();
  SDL_RenderPresent(renderer_);
  SDL_Delay(1000/60);
}


SDL_Window* Renderer::window() const {
  return window_;
}


} // namespace Tetris
