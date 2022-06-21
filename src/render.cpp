#include "render.hpp"

#include <map>

#include "piece.hpp"


namespace Tetris {


// RenderState
int RenderState::dx() const { return dx_; }
int RenderState::dy() const { return dy_; }
int RenderState::grid_width() const { return grid_width_; }
int RenderState::grid_height() const { return grid_height_; }


// PlayingState
void PlayingState::Render(
    const Game& g, SDL_Renderer* r) {

  // Render grid
  SDL_SetRenderDrawColor(r, 0, 0, 0, 255);

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

  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);

  using ColorMap = std::map<Tetris::PieceType, std::vector<uint8_t>>;
  ColorMap colors;
  colors[Tetris::PieceType::I] = {100, 0, 0, 255};
  colors[Tetris::PieceType::J] = {100, 100, 0, 255};
  colors[Tetris::PieceType::L] = {100, 0, 100, 255};
  colors[Tetris::PieceType::O] = {0, 100, 100, 255};
  colors[Tetris::PieceType::S] = {0, 100, 0, 255};
  colors[Tetris::PieceType::T] = {0, 0, 100, 255};
  colors[Tetris::PieceType::Z] = {100, 0, 0, 255};

  // Render current piece
  const auto cur_type = g.current_piece->type;
  SDL_SetRenderDrawColor(
      r, 
      colors[cur_type][0],
      colors[cur_type][1],
      colors[cur_type][2],
      colors[cur_type][3]);

  for (const auto& p : g.current_piece->points) {
    const auto row = p.first;
    const auto col = p.second;
    SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }

  // Render destination
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  for (const auto& p : g.GetDestination()) {
    const auto row = p.first;
    const auto col = p.second;
    SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
    SDL_RenderDrawRect(r, &rect);
  }

  // Render next piece
  const auto next_type = g.next_piece->type;
  SDL_SetRenderDrawColor(
      r, 
      colors[next_type][0],
      colors[next_type][1],
      colors[next_type][2],
      colors[next_type][3]);

  for (const auto& p : g.next_piece->points) {
    const auto row = p.first;
    const auto col = p.second;
    SDL_Rect rect{
      grid_width() + col*dx() + 1, (row+5)*dy() + 1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }

  SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
  SDL_Rect frame{
    grid_width() + 2*dx(), 3*dy(), 6*dx(), 6*dy()};
  SDL_RenderDrawRect(r, &frame);

  // Render board
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  for (int row = 0; row < g.board.rows; ++row) {
    for (int col = 0; col < g.board.cols; ++col) {
      if (g.board.matrix[row][col] == 0) { continue; }

      SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
      SDL_RenderFillRect(r, &rect);
    }
  }
}


std::unique_ptr<RenderState> PlayingState::Transition(const Game& g) {
  auto cleared_rows = g.GetClearedRows();
  if (g.GameOver()) {
    return std::make_unique<GameOverState>();
  }

  if (cleared_rows.size() > 0) {
    return std::make_unique<RowClearState>(cleared_rows);
  }

  return nullptr;
}


// RowClearState
RowClearState::RowClearState(std::vector<int> rows_cleared)
    : rows_cleared_{rows_cleared}, ticks_{SDL_GetTicks64()} {}


void RowClearState::Enter(const Game& g, SDL_Renderer* r) {
  while (SDL_GetTicks64() < ticks_ + animation_milliseconds_) {
    ((SDL_GetTicks64() - ticks_) / 100) % 2 == 0 ?
      SDL_SetRenderDrawColor(r, 255, 0, 0, 255) :
      SDL_SetRenderDrawColor(r, 0, 255, 0, 255);

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


std::unique_ptr<RenderState> RowClearState::Transition(const Game& g) {
  if (SDL_GetTicks64() > ticks_ + animation_milliseconds_) {
    return std::make_unique<PlayingState>();
  }
  else {
    return nullptr;
  }
}


// GameOverState
GameOverState::GameOverState() : ticks_{SDL_GetTicks64()} {}


void GameOverState::FillRow(const Game& g, SDL_Renderer* r, int row) {
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  for (int col = 0; col < g.board.cols; ++col) {
    SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }
}


void GameOverState::Enter(const Game& g, SDL_Renderer* r) {

  while (SDL_GetTicks64() < ticks_ + animation_milliseconds_) {
    const auto elapsed = (SDL_GetTicks64() - ticks_) /
      static_cast<double>(animation_milliseconds_);
    const int row = (g.board.rows-1)*(1-elapsed);

    FillRow(g, r, row);

    SDL_RenderPresent(r);
    SDL_Delay(1000/60);
  }
}


void GameOverState::Render(const Game& g, SDL_Renderer* r) {
  PlayingState::Render(g, r);
  for (int row = 0; row < g.board.rows; ++row) {
    FillRow(g, r, row);
  }
}


std::unique_ptr<RenderState> GameOverState::Transition(const Game& g) {
  if (!g.GameOver()) {
    return std::make_unique<PlayingState>();
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

  // Initialize SDL_ttf
  if (TTF_Init() == -1) {
    printf("TTF_Init: %s\n", TTF_GetError());
  }

  // Set playing state
  state_ = std::make_unique<PlayingState>();
}


Renderer::~Renderer() {
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  TTF_Quit();
  SDL_Quit();
}


void Renderer::Render() {
  auto new_state = state_->Transition(game_);

  if (new_state) {
    state_ = std::move(new_state);
    state_->Enter(game_, renderer_);
  }

  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderClear(renderer_);
  state_->Render(game_, renderer_);
  SDL_RenderPresent(renderer_);
  SDL_Delay(1000/60);
}


SDL_Window* Renderer::window() const {
  return window_;
}


} // namespace Tetris