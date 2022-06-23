#include "render_states.hpp"

#include <map>
#include <string>

#include "piece.hpp"


namespace Tetris {


// RenderState
RenderStateBase::RenderStateBase(RenderInfo info) : info{info} {}


int RenderStateBase::dx() const { return dx_; }
int RenderStateBase::dy() const { return dy_; }
int RenderStateBase::grid_width() const { return grid_width_; }
int RenderStateBase::grid_height() const { return grid_height_; }
RenderStateName RenderStateBase::name() const { return name_; }
void RenderStateBase::set_name(RenderStateName name) { name_ = name; }


// RenderStatePlaying
RenderStatePlaying::RenderStatePlaying(RenderInfo info)
    : RenderStateBase{info} {
  piece_colors_[Tetris::PieceType::I] = {0, 200, 200, 255};
  piece_colors_[Tetris::PieceType::J] = {0, 0, 200, 255};
  piece_colors_[Tetris::PieceType::L] = {255, 165, 100, 255};
  piece_colors_[Tetris::PieceType::O] = {255, 255, 100, 255};
  piece_colors_[Tetris::PieceType::S] = {0, 200, 0, 255};
  piece_colors_[Tetris::PieceType::T] = {231, 84, 128, 255};
  piece_colors_[Tetris::PieceType::Z] = {200, 0, 0, 255};
  set_name(RenderStateName::PLAYING);
}


void RenderStatePlaying::FillRow(int row) {
  const Game& g = info.game;
  SDL_Renderer* r = info.renderer;

  const auto fill = filled_color_;
  SDL_SetRenderDrawColor(r, fill.r, fill.g, fill.b, fill.a);
  for (int col = 0; col < g.board.cols; ++col) {
    SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }
}


void RenderStatePlaying::FillBoard() {
  for (int row = 0; row < info.game.board.rows; ++row) {
    FillRow(row);
  }
}


void RenderStatePlaying::DrawGrid() {
  const auto color = grid_lines_color_;
  auto r = info.renderer;
  const auto& g = info.game;
  SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

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
}


void RenderStatePlaying::DrawCurrentPiece() {
  auto r = info.renderer;
  const auto& g = info.game;
  const auto cur_type = g.current_piece->type;

  SDL_SetRenderDrawColor(
      r,
      piece_colors_[cur_type].r,
      piece_colors_[cur_type].g,
      piece_colors_[cur_type].b,
      piece_colors_[cur_type].a);

  for (const auto& p : *g.current_piece) {
    const auto row = p.row;
    const auto col = p.col;
    SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }
}


void RenderStatePlaying::DrawDestination() {
  auto r = info.renderer;
  const auto& g = info.game;
  const auto cur_type = g.current_piece->type;
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
}


void RenderStatePlaying::DrawNextPiece() {
  auto r = info.renderer;
  const auto& g = info.game;

  const auto next_type = g.next_piece->type;
  SDL_SetRenderDrawColor(
      r, 
      piece_colors_[next_type].r,
      piece_colors_[next_type].g,
      piece_colors_[next_type].b,
      piece_colors_[next_type].a);

  for (const auto& p : *g.next_piece) {
    const auto row = p.row;
    const auto col = p.col;
    SDL_Rect rect{
      grid_width() + (col-1)*dx() + 1, (row+5)*dy() + 1, dx()-2, dy()-2};
    SDL_RenderFillRect(r, &rect);
  }

  const auto c = grid_lines_color_;
  SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
  SDL_Rect frame{
    grid_width() + dx(), 3*dy(), 6*dx(), 6*dy()};
  SDL_RenderDrawRect(r, &frame);
}


void RenderStatePlaying::DrawBoard() {
  auto r = info.renderer;
  const auto& g = info.game;

  const auto c = filled_color_;
  SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
  for (int row = 0; row < g.board.rows; ++row) {
    for (int col = 0; col < g.board.cols; ++col) {
      if (g.board.matrix[row][col] == 0) { continue; }

      SDL_Rect rect{col*dx()+1, row*dy()+1, dx()-2, dy()-2};
      SDL_RenderFillRect(r, &rect);
    }
  }
}


void RenderStatePlaying::DrawScore() {
  auto r = info.renderer;
  const auto& g = info.game;

  const auto score_str = "Score: " + std::to_string(g.score());
  const auto level_str = "Level: " + std::to_string(g.level());
  const auto str = level_str + "\n" + score_str;
  FC_DrawAlign(
      info.font,
      r,
      grid_width() + 4*dx(),
      10*dy(),
      FC_ALIGN_CENTER,
      str.c_str());
}


void RenderStatePlaying::Render() {

  const Game& g = info.game;
  SDL_Renderer* r = info.renderer;

  // Set background
  const auto bg = bg_color_;
  SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, bg.a);
  SDL_RenderClear(r);

  DrawGrid();
  DrawBoard();
  DrawCurrentPiece();
  DrawDestination();
  DrawNextPiece();
  DrawScore();
}


std::unique_ptr<RenderStateBase> RenderStatePlaying::Transition() {
  const Game& g = info.game;

  auto cleared_rows = g.GetClearedRows();
  if (g.gameover()) {
    return std::make_unique<RenderStateGameOver>(info);
  }

  if (cleared_rows.size() > 0) {
    return std::make_unique<RenderStateRowClear>(info, cleared_rows);
  }

  if (g.paused()) {
    return std::make_unique<RenderStatePaused>(info);
  }

  return nullptr;
}


SDL_Color RenderStatePlaying::bg_color() const { return bg_color_; }
SDL_Color RenderStatePlaying::filled_color() const { return filled_color_; }
SDL_Color RenderStatePlaying::grid_color() const { return grid_lines_color_; }


// RenderStateRowClear
RenderStateRowClear::RenderStateRowClear(
    RenderInfo info, std::vector<int> rows_cleared)
    : RenderStatePlaying{info}
    , rows_cleared_{rows_cleared}
    , ticks_{SDL_GetTicks64()} {
  set_name(RenderStateName::ROWCLEAR);
}


void RenderStateRowClear::Enter() {
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


std::unique_ptr<RenderStateBase> RenderStateRowClear::Transition() {
  const Game& g = info.game;

  if (SDL_GetTicks64() > ticks_ + animation_milliseconds_) {
    return std::make_unique<RenderStatePlaying>(info);
  }
  else {
    return nullptr;
  }
}


// RenderStateGameOver
RenderStateGameOver::RenderStateGameOver(RenderInfo info)
    : RenderStatePlaying{info}
    , ticks_{SDL_GetTicks64()} {
  set_name(RenderStateName::GAMEOVER);
}


void RenderStateGameOver::Enter() {
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


void RenderStateGameOver::Render() {
  const Game& g = info.game;
  SDL_Renderer* r = info.renderer;

  RenderStatePlaying::Render();
  FillBoard();

  // Render text
  const std::string str = "Game Over!\n\nPress R to restart.";
  FC_DrawAlign(info.font, r, 5*dx(), 10*dy(), FC_ALIGN_CENTER, str.c_str());
}


std::unique_ptr<RenderStateBase> RenderStateGameOver::Transition() {
  const Game& g = info.game;

  if (!g.gameover()) {
    return std::make_unique<RenderStatePlaying>(info);
  }
  else {
    return nullptr;
  }
}


// RenderStatePaused
RenderStatePaused::RenderStatePaused(RenderInfo info)
    : RenderStatePlaying{info} {
  set_name(RenderStateName::PAUSED);
}


void RenderStatePaused::Render() {
  RenderStatePlaying::Render();
  FillBoard();

  // Render text
  FC_DrawAlign(
      info.font,
      info.renderer,
      5*dx(),
      10*dy(),
      FC_ALIGN_CENTER,
      "Paused");
}


std::unique_ptr<RenderStateBase> RenderStatePaused::Transition() {
  const Game& g = info.game;

  if (!g.paused()) {
    return std::make_unique<RenderStatePlaying>(info);
  }
  else {
    return nullptr;
  }
}


} // namespace Tetris
