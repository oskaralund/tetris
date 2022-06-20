#include "piece.hpp"

#include <algorithm>

#include "board.hpp"


namespace Tetris {


Piece::Piece(Board* board) 
  : board{board}
{}


// LPiece
LPiece::LPiece(Board* board) : Piece{board} {
  points = {{0, 4}, {1, 4}, {2, 4}, {2, 5}};
  prev_points = points;
  type = PieceType::L;
}


void LPiece::RotateCW() {
  Points new_points{4};
  const Point center = points[1];
  switch (rotation) {
    case 0:
      new_points[0] = {center.first, center.second+1};
      new_points[1] = center;
      new_points[2] = {center.first, center.second-1};
      new_points[3] = {center.first+1, center.second-1};
      break;
    case 1:
      new_points[0] = {center.first+1, center.second};
      new_points[1] = center;
      new_points[2] = {center.first-1, center.second};
      new_points[3] = {center.first-1, center.second-1};
      break;
    case 2:
      new_points[0] = {center.first, center.second-1};
      new_points[1] = center;
      new_points[2] = {center.first, center.second+1};
      new_points[3] = {center.first-1, center.second+1};
      break;
    case 3:
      new_points[0] = {center.first-1, center.second};
      new_points[1] = center;
      new_points[2] = {center.first+1, center.second};
      new_points[3] = {center.first+1, center.second+1};
      break;
  }

  rotation += 1;
  rotation %= 4;
  points = new_points;
}


// JPiece
JPiece::JPiece(Board* board) : Piece{board} {
  points = {{0, 4}, {1, 4}, {2, 4}, {2, 3}};
  prev_points = points;
  type = PieceType::J;
}


void JPiece::RotateCW() {
  Points new_points{4};
  const Point center = points[1];
  switch (rotation) {
    case 0:
      new_points[0] = {center.first, center.second+1};
      new_points[1] = center;
      new_points[2] = {center.first, center.second-1};
      new_points[3] = {center.first-1, center.second-1};
      break;
    case 1:
      new_points[0] = {center.first+1, center.second};
      new_points[1] = center;
      new_points[2] = {center.first-1, center.second};
      new_points[3] = {center.first-1, center.second+1};
      break;
    case 2:
      new_points[0] = {center.first, center.second-1};
      new_points[1] = center;
      new_points[2] = {center.first, center.second+1};
      new_points[3] = {center.first+1, center.second+1};
      break;
    case 3:
      new_points[0] = {center.first-1, center.second};
      new_points[1] = center;
      new_points[2] = {center.first+1, center.second};
      new_points[3] = {center.first+1, center.second-1};
      break;
  }

  rotation += 1;
  rotation %= 4;
  points = new_points;
}


// IPiece
IPiece::IPiece(Board* board) : Piece{board} {
  points = {{0, 4}, {1, 4}, {2, 4}, {3, 4}};
  prev_points = points;
  type = PieceType::I;
}


void IPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[0];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.first+1, anchor.second-2};
      new_points[1] = {anchor.first+1, anchor.second-1};
      new_points[2] = {anchor.first+1, anchor.second};
      new_points[3] = {anchor.first+1, anchor.second+1};
      break;
    case 1:
      new_points[0] = {anchor.first-1, anchor.second+2};
      new_points[1] = {anchor.first  , anchor.second+2};
      new_points[2] = {anchor.first+1, anchor.second+2};
      new_points[3] = {anchor.first+2, anchor.second+2};
      break;
  }

  rotation += 1;
  rotation %= 2;
  points = new_points;
}


// OPiece
OPiece::OPiece(Board* board) : Piece{board} {
  points = {{0, 4}, {0, 5}, {1, 4}, {1, 5}};
  prev_points = points;
  type = PieceType::O;
}


void OPiece::RotateCW() {
  return;
}


// SPiece
SPiece::SPiece(Board* board) : Piece{board} {
  points = {{1, 4}, {1, 5}, {0, 5}, {0, 6}};
  prev_points = points;
  type = PieceType::S;
}


void SPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[1];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.first-1, anchor.second};
      new_points[1] = anchor;
      new_points[2] = {anchor.first, anchor.second+1};
      new_points[3] = {anchor.first+1, anchor.second+1};
      break;
    case 1:
      new_points[0] = {anchor.first, anchor.second-1};
      new_points[1] = anchor;
      new_points[2] = {anchor.first-1, anchor.second};
      new_points[3] = {anchor.first-1, anchor.second+1};
      break;
  }

  rotation += 1;
  rotation %= 2;
  points = new_points;
}


// TPiece
TPiece::TPiece(Board* board) : Piece{board} {
  points = {{0, 5}, {1, 4}, {1, 5}, {1, 6}};
  prev_points = points;
  type = PieceType::T;
}


void TPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[0];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.first, anchor.second+1};
      new_points[1] = {anchor.first-1, anchor.second};
      new_points[2] = anchor;
      new_points[3] = {anchor.first+1, anchor.second};
      break;
    case 1:
      new_points[0] = {anchor.first+1, anchor.second-1};
      new_points[1] = anchor;
      new_points[2] = {anchor.first, anchor.second-1};
      new_points[3] = {anchor.first, anchor.second-2};
      break;
    case 2:
      new_points[0] = {anchor.first-1, anchor.second-1};
      new_points[1] = anchor;
      new_points[2] = {anchor.first-1, anchor.second};
      new_points[3] = {anchor.first-2, anchor.second};
      break;
    case 3:
      new_points[0] = {anchor.first, anchor.second+1};
      new_points[1] = {anchor.first+1, anchor.second};
      new_points[2] = {anchor.first+1, anchor.second+1};
      new_points[3] = {anchor.first+1, anchor.second+2};
      break;
  }

  rotation += 1;
  rotation %= 4;
  points = new_points;
}


// ZPiece
ZPiece::ZPiece(Board* board) : Piece{board} {
  points = {{0, 4}, {0, 5}, {1, 5}, {1, 6}};
  prev_points = points;
  type = PieceType::Z;
}


void ZPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[1];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.first-1, anchor.second};
      new_points[1] = anchor;
      new_points[2] = {anchor.first, anchor.second-1};
      new_points[3] = {anchor.first+1, anchor.second-1};
      break;
    case 1:
      new_points[0] = {anchor.first, anchor.second-1};
      new_points[1] = anchor;
      new_points[2] = {anchor.first+1, anchor.second};
      new_points[3] = {anchor.first+1, anchor.second+1};
      break;
  }

  rotation += 1;
  rotation %= 2;
  points = new_points;
}


} // namespace Tetris
