#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "piece.hpp"
#include "game.hpp"
#include "tetris.hpp"


const int DX = 25;
const int DY = 25;
const int GRID_WIDTH = 10*DX;
const int GRID_HEIGHT = 24*DY;
const int SCREEN_WIDTH = GRID_WIDTH + 10*DX;
const int SCREEN_HEIGHT = GRID_HEIGHT;
TTF_Font* FONT = nullptr;


void DrawGame(SDL_Renderer*, const Tetris::Game&);
void DrawGrid(SDL_Renderer*);
void RunClearRowsAnimation(
    SDL_Renderer*,
    const Tetris::Game&,
    Tetris::ClearedRows);
void HandleEvent(const SDL_Event&, SDL_Window*, SDL_Renderer*, Tetris::Game*);
bool PollEvents(SDL_Window*, SDL_Renderer*, Tetris::Game*);


int main() {

  Tetris::Game tetris;

  // Initialize SDL
  auto init = SDL_Init(SDL_INIT_VIDEO);
  if ( init < 0 ) {
      printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
      return -1;
  }

  // Initialize SDL_ttf
  if(TTF_Init()==-1) {
      printf("TTF_Init: %s\n", TTF_GetError());
      return -1;
  }
  //FONT=TTF_OpenFont("arial.ttf", 16);
  //if(!FONT) {
  //    printf("TTF_OpenFont: %s\n", TTF_GetError());
  //    return -1;
  //}

  // Create window
  SDL_Window* window = SDL_CreateWindow(
      "Tetris",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_SHOWN);

  if (window == NULL) {
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
      return -1;
  }

  // Create renderer
  SDL_Renderer* renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
      printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
      return -1;
  }

  // Game loop
  auto time = SDL_GetTicks();
  while (true) {
    if (tetris.GameOver()) {
      tetris.Restart();
    }

    bool quit = PollEvents(window, renderer, &tetris);
    if (quit) { break; }

    auto elapsed_time = (SDL_GetTicks()-time) / 1000.0;
    time = SDL_GetTicks();

    auto cleared_rows = tetris.Step(elapsed_time);

    if (cleared_rows.size() > 0) {
      RunClearRowsAnimation(renderer, tetris, cleared_rows);
      time -= 1000;
    }


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    DrawGame(renderer, tetris);
    SDL_RenderPresent(renderer);
  }


  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  return 0;
}


void DrawGame(SDL_Renderer* renderer, const Tetris::Game& game) {
  // Render grid
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  // Render vertical lines
  for (int i = 0; i <= game.board.cols; ++i) {
    auto x1 = i*DX;
    auto x2 = x1;
    auto y1 = 0;
    auto y2 = GRID_HEIGHT;
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }

  // Render horizontal lines
  for (int i = 0; i <= game.board.rows; ++i) {
    auto x1 = 0;
    auto x2 = GRID_WIDTH;
    auto y1 = i*DY;
    auto y2 = y1;
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

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
  const auto cur_type = game.current_piece->type;
  SDL_SetRenderDrawColor(
      renderer, 
      colors[cur_type][0],
      colors[cur_type][1],
      colors[cur_type][2],
      colors[cur_type][3]);

  for (const auto& p : game.current_piece->points) {
    const auto row = p.first;
    const auto col = p.second;
    SDL_Rect rect{col*DX+1, row*DY+1, DX-2, DY-2};
    SDL_RenderFillRect(renderer, &rect);
  }

  // Render destination
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (const auto& p : game.GetDestination()) {
    const auto row = p.first;
    const auto col = p.second;
    SDL_Rect rect{col*DX+1, row*DY+1, DX-2, DY-2};
    SDL_RenderDrawRect(renderer, &rect);
  }

  // Render next piece
  const auto next_type = game.next_piece->type;
  SDL_SetRenderDrawColor(
      renderer, 
      colors[next_type][0],
      colors[next_type][1],
      colors[next_type][2],
      colors[next_type][3]);

  for (const auto& p : game.next_piece->points) {
    const auto row = p.first;
    const auto col = p.second;
    SDL_Rect rect{
      GRID_WIDTH + col*DX + 1, (row+5)*DY + 1, DX-2, DY-2};
    SDL_RenderFillRect(renderer, &rect);
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_Rect frame{
    GRID_WIDTH + 2*DX, 3*DY, 6*DX, 6*DY};
  SDL_RenderDrawRect(renderer, &frame);

  // Render board
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (int row = 0; row < game.board.rows; ++row) {
    for (int col = 0; col < game.board.cols; ++col) {
      if (game.board.matrix[row][col] == 0) { continue; }

      SDL_Rect rect{col*DX+1, row*DY+1, DX-2, DY-2};
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}


void RunClearRowsAnimation(
    SDL_Renderer* renderer,
    const Tetris::Game& game,
    Tetris::ClearedRows rows) {

  auto start_time = SDL_GetTicks64();

  while (SDL_GetTicks64() < start_time + 1000) {

    ((SDL_GetTicks64() - start_time) / 100) % 2 == 0 ?
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255) :
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for (const auto& row : rows) {
      for (int col = 0; col < game.board.cols; ++col) {
        SDL_Rect rect{col*DX+1, row*DY+1, DX-2, DY-2};
        SDL_RenderFillRect(renderer, &rect);
      }
    }

    SDL_RenderPresent(renderer);
  }
}


void HandleEvent(
    const SDL_Event& e,
    SDL_Window* w,
    SDL_Renderer* r,
    Tetris::Game* g) {

  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_LEFT:
        g->Left();
        break;
      case SDLK_RIGHT:
        g->Right();
        break;
      case SDLK_DOWN:
        g->QuickDrop(true);
        break;
      case SDLK_UP:
        g->RotateCW();
        break;
      case SDLK_SPACE:
        g->Down();
        break;
    }
  }

  if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {
      case SDLK_DOWN:
        g->QuickDrop(false);
        break;
    }
  }
}


bool PollEvents(
    SDL_Window* w,
    SDL_Renderer* r,
    Tetris::Game* g) {
  SDL_Event e;

  auto pending = SDL_PollEvent(&e);
  while (pending) {
    if (e.type == SDL_QUIT) {
      return true;
    }
    HandleEvent(e, w, r, g);
    pending = SDL_PollEvent(&e);
  }

  return false;
}

