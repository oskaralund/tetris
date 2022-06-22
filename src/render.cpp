#include "render.hpp"

#include <map>
#include <string>

#include "piece.hpp"


namespace Tetris {


// RenderState
int RenderState::dx() const { return dx_; }
int RenderState::dy() const { return dy_; }
int RenderState::grid_width() const { return grid_width_; }
int RenderState::grid_height() const { return grid_height_; }

RenderState::RenderState(RenderInfo info) : info{info} {}


// PlayingState
PlayingState::PlayingState(RenderInfo info) : RenderState{info} {
  piece_colors_[Tetris::PieceType::I] = {0, 200, 200, 255};
  piece_colors_[Tetris::PieceType::J] = {0, 0, 200, 255};
  piece_colors_[Tetris::PieceType::L] = {255, 165, 100, 255};
  piece_colors_[Tetris::PieceType::O] = {255, 255, 100, 255};
  piece_colors_[Tetris::PieceType::S] = {0, 200, 0, 255};
  piece_colors_[Tetris::PieceType::T] = {231, 84, 128, 255};
  piece_colors_[Tetris::PieceType::Z] = {200, 0, 0, 255};
}


void PlayingState::Render() {

  const Game& g = info.game;
  SDL_Renderer* r = info.renderer;

  // Set background
  SDL_SetRenderDrawColor(r, bg_.r, bg_.g, bg_.b, bg_.a);
  SDL_RenderClear(r);

  // Render grid
  SDL_SetRenderDrawColor(r, grid_.r, grid_.g, grid_.b, grid_.a);

  // Render vertical lines
  for (int i = 0; i <= g.board.cols; ++i) {
    auto x1 = i*dx();
    auto x2 = x1;
    auto y1 = 0;
    auto y2 = grid_height();
    SDL_RenderDrawLine(r, x1, y1, x2, y2);
  }

  // Render horizontal lines
  for (int i = 0; i <= g.board.rows; ++i) {
    auto x1 = 0;
    auto x2 = grid_width();
    auto y1 = i*dy();
    auto y2 = y1;
    SDL_RenderDrawLine(r, x1, y1, x2, y2);
  }

  // Render current piece
  const auto cur_type = g.current_piece->type;
  SDL_SetRenderDrawColor(
      r,
      piece_colors_[cur_type].r,
      piece_colors_[cur_type].g,
      piece_colors_[cur_type].b,
      piece_colors_[cur_type].a);

  for (const auto& p : g.current_piece->points) {
    const auto row = p.row;
    const auto col = p.col;
    SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }

  // Render destination
  SDL_SetRenderDrawColor(
      r, 
      piece_colors_[cur_type].r,
      piece_colors_[cur_type].g,
      piece_colors_[cur_type].b,
      50);
  for (const auto& p : g.GetDestination()) {
    const auto row = p.row;
    const auto col = p.col;
    SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }

  // Render next piece
  const auto next_type = g.next_piece->type;
  SDL_SetRenderDrawColor(
      r, 
      piece_colors_[next_type].r,
      piece_colors_[next_type].g,
      piece_colors_[next_type].b,
      piece_colors_[next_type].a);

  for (const auto& p : g.next_piece->points) {
    const auto row = p.row;
    const auto col = p.col;
    SDL_Rect rect{
      grid_width() + col*dx() + 1, (row+5)*dy() + 1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }

  SDL_SetRenderDrawColor(r, grid_.r, grid_.g, grid_.b, grid_.a);
  SDL_Rect frame{
    grid_width() + 2*dx(), 3*dy(), 6*dx(), 6*dy()};
  SDL_RenderDrawRect(r, &frame);

  // Render board
  SDL_SetRenderDrawColor(r, filled_.r, filled_.g, filled_.b, filled_.a);
  for (int row = 0; row < g.board.rows; ++row) {
    for (int col = 0; col < g.board.cols; ++col) {
      if (g.board.matrix[row][col] == 0) { continue; }

      SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
      SDL_RenderFillRect(r, &rect);
    }
  }

  // Render score
  const auto score_str = "Score: " + std::to_string(info.game.score());
  const auto level_str = "Level: " + std::to_string(info.game.level());
  const auto str = level_str + "\n" + score_str;
  FC_DrawAlign(
      info.font,
      r,
      grid_width() + 4*dx(),
      10*dy(),
      FC_ALIGN_CENTER,
      str.c_str());
}


std::unique_ptr<RenderState> PlayingState::Transition() {
  const Game& g = info.game;

  auto cleared_rows = g.GetClearedRows();
  if (g.gameover()) {
    return std::make_unique<GameOverState>(info);
  }

  if (cleared_rows.size() > 0) {
    return std::make_unique<RowClearState>(info, cleared_rows);
  }

  return nullptr;
}


SDL_Color PlayingState::bg_color() const { return bg_; }
SDL_Color PlayingState::filled_color() const { return filled_; }
SDL_Color PlayingState::grid_color() const { return grid_; }


// RowClearState
RowClearState::RowClearState(RenderInfo info, std::vector<int> rows_cleared)
    : PlayingState{info}
    , rows_cleared_{rows_cleared}
    , ticks_{SDL_GetTicks64()} {}


void RowClearState::Enter() {
  const Game& g = info.game;
  SDL_Renderer* r = info.renderer;

  const auto bg = bg_color();
  const auto fill = filled_color();

  while (SDL_GetTicks64() < ticks_ + animation_milliseconds_) {
    ((SDL_GetTicks64() - ticks_) / 100) % 2 == 0 ?
      SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, bg.a) :
      SDL_SetRenderDrawColor(r, fill.r, fill.g, fill.b, fill.a);

    for (const auto& row : rows_cleared_) {
      for (int col = 0; col < g.board.cols; ++col) {
        SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
        SDL_RenderFillRect(r, &rect);
      }
    }

    SDL_Delay(1000/60);
    SDL_RenderPresent(r);
  }
}


std::unique_ptr<RenderState> RowClearState::Transition() {
  const Game& g = info.game;

  if (SDL_GetTicks64() > ticks_ + animation_milliseconds_) {
    return std::make_unique<PlayingState>(info);
  }
  else {
    return nullptr;
  }
}


// GameOverState
GameOverState::GameOverState(RenderInfo info)
    : PlayingState{info}
    , ticks_{SDL_GetTicks64()} {}


void GameOverState::FillRow(int row) {
  const Game& g = info.game;
  SDL_Renderer* r = info.renderer;

  const auto fill = filled_color();
  SDL_SetRenderDrawColor(r, fill.r, fill.g, fill.b, fill.a);
  for (int col = 0; col < g.board.cols; ++col) {
    SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }
}


void GameOverState::Enter() {
  const Game& g = info.game;
  SDL_Renderer* r = info.renderer;

  while (SDL_GetTicks64() < ticks_ + animation_milliseconds_) {
    const auto elapsed = (SDL_GetTicks64() - ticks_) /
      static_cast<double>(animation_milliseconds_);
    const int row = (g.board.rows-1)*(1-elapsed);

    FillRow(row);

    SDL_RenderPresent(r);
    SDL_Delay(1000/60);
  }
}


void GameOverState::Render() {
  const Game& g = info.game;
  SDL_Renderer* r = info.renderer;

  PlayingState::Render();
  for (int row = 0; row < g.board.rows; ++row) {
    FillRow(row);
  }

  // Render text
  const std::string str = "Game Over!\n\nPress R to restart.";
  FC_DrawAlign(info.font, r, 5*dx(), 10*dy(), FC_ALIGN_CENTER, str.c_str());
}


std::unique_ptr<RenderState> GameOverState::Transition() {
  const Game& g = info.game;

  if (!g.gameover()) {
    return std::make_unique<PlayingState>(info);
  }
  else {
    return nullptr;
  }
}


// Renderer
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

  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderClear(renderer_);
  state_->Render();
  SDL_RenderPresent(renderer_);
  SDL_Delay(1000/60);
}


SDL_Window* Renderer::window() const {
  return window_;
}


} // namespace Tetris
