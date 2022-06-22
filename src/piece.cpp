#include "piece.hpp"

#include <algorithm>

#include "board.hpp"


namespace Tetris {


void Piece::Translate(int rows, int cols) {
  for (auto& p : points) {
    p.row += rows;
    p.col += cols;
  }
}


// IPiece
IPiece::IPiece() {
  points = {{1, 3}, {1, 4}, {1, 5}, {1, 6}};
  prev_points = points;
  type = PieceType::I;
}


void IPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[0];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.row-1, anchor.col+2};
      new_points[1] = {anchor.row  , anchor.col+2};
      new_points[2] = {anchor.row+1, anchor.col+2};
      new_points[3] = {anchor.row+2, anchor.col+2};
      break;
    case 1:
      new_points[0] = {anchor.row+2, anchor.col+1};
      new_points[1] = {anchor.row+2, anchor.col  };
      new_points[2] = {anchor.row+2, anchor.col-1};
      new_points[3] = {anchor.row+2, anchor.col-2};
      break;
    case 2:
      new_points[0] = {anchor.row+1, anchor.col-2};
      new_points[1] = {anchor.row  , anchor.col-2};
      new_points[2] = {anchor.row-1, anchor.col-2};
      new_points[3] = {anchor.row-2, anchor.col-2};
      break;
    case 3:
      new_points[0] = {anchor.row-2, anchor.col-1};
      new_points[1] = {anchor.row-2, anchor.col  };
      new_points[2] = {anchor.row-2, anchor.col+1};
      new_points[3] = {anchor.row-2, anchor.col+2};
      break;
  }

  rotation += 1;
  rotation %= 4;
  points = new_points;
}


// JPiece
JPiece::JPiece() {
  points = {{0, 4}, {1, 4}, {1, 5}, {1, 6}};
  prev_points = points;
  type = PieceType::J;
}


void JPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[0];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.row  , anchor.col+2};
      new_points[1] = {anchor.row  , anchor.col+1};
      new_points[2] = {anchor.row+1, anchor.col+1};
      new_points[3] = {anchor.row+2, anchor.col+1};
      break;
    case 1:
      new_points[0] = {anchor.row+2, anchor.col  };
      new_points[1] = {anchor.row+1, anchor.col  };
      new_points[2] = {anchor.row+1, anchor.col-1};
      new_points[3] = {anchor.row+1, anchor.col-2};
      break;
    case 2:
      new_points[0] = {anchor.row  , anchor.col-2};
      new_points[1] = {anchor.row  , anchor.col-1};
      new_points[2] = {anchor.row-1, anchor.col-1};
      new_points[3] = {anchor.row-2, anchor.col-1};
      break;
    case 3:
      new_points[0] = {anchor.row-2, anchor.col  };
      new_points[1] = {anchor.row-1, anchor.col  };
      new_points[2] = {anchor.row-1, anchor.col+1};
      new_points[3] = {anchor.row-1, anchor.col+2};
      break;
  }

  rotation += 1;
  rotation %= 4;
  points = new_points;
}


// LPiece
LPiece::LPiece() {
  points = {{1, 4}, {1, 5}, {1, 6}, {0, 6}};
  prev_points = points;
  type = PieceType::L;
}


void LPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[0];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.row-1, anchor.col+1};
      new_points[1] = {anchor.row  , anchor.col+1};
      new_points[2] = {anchor.row+1, anchor.col+1};
      new_points[3] = {anchor.row+1, anchor.col+2};
      break;
    case 1:
      new_points[0] = {anchor.row+1, anchor.col+1};
      new_points[1] = {anchor.row+1, anchor.col  };
      new_points[2] = {anchor.row+1, anchor.col-1};
      new_points[3] = {anchor.row+2, anchor.col-1};
      break;
    case 2:
      new_points[0] = {anchor.row+1, anchor.col-1};
      new_points[1] = {anchor.row  , anchor.col-1};
      new_points[2] = {anchor.row-1, anchor.col-1};
      new_points[3] = {anchor.row-1, anchor.col-2};
      break;
    case 3:
      new_points[0] = {anchor.row-1, anchor.col-1};
      new_points[1] = {anchor.row-1, anchor.col  };
      new_points[2] = {anchor.row-1, anchor.col+1};
      new_points[3] = {anchor.row-2, anchor.col+1};
      break;
  }

  rotation += 1;
  rotation %= 4;
  points = new_points;
}


// OPiece
OPiece::OPiece() {
  points = {{0, 4}, {0, 5}, {1, 4}, {1, 5}};
  prev_points = points;
  type = PieceType::O;
}


void OPiece::RotateCW() {
  return;
}


// SPiece
SPiece::SPiece() {
  points = {{1, 4}, {1, 5}, {0, 5}, {0, 6}};
  prev_points = points;
  type = PieceType::S;
}


void SPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[0];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.row-1, anchor.col+1};
      new_points[1] = {anchor.row  , anchor.col+1};
      new_points[2] = {anchor.row  , anchor.col+2};
      new_points[3] = {anchor.row+1, anchor.col+2};
      break;
    case 1:
      new_points[0] = {anchor.row+2, anchor.col-1};
      new_points[1] = {anchor.row+2, anchor.col  };
      new_points[2] = {anchor.row+1, anchor.col  };
      new_points[3] = {anchor.row+1, anchor.col+1};
      break;
    case 2:
      new_points[0] = {anchor.row-2, anchor.col  };
      new_points[1] = {anchor.row-1, anchor.col  };
      new_points[2] = {anchor.row-1, anchor.col+1};
      new_points[3] = {anchor.row  , anchor.col+1};
      break;
    case 3:
      new_points[0] = {anchor.row+1, anchor.col  };
      new_points[1] = {anchor.row+1, anchor.col+1};
      new_points[2] = {anchor.row  , anchor.col+1};
      new_points[3] = {anchor.row  , anchor.col+2};
      break;
  }

  rotation += 1;
  rotation %= 4;
  points = new_points;
}


// TPiece
TPiece::TPiece() {
  points = {{1, 4}, {1, 5}, {1, 6}, {0, 5}};
  prev_points = points;
  type = PieceType::T;
}


void TPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[0];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.row-1, anchor.col+1};
      new_points[1] = {anchor.row  , anchor.col+1};
      new_points[2] = {anchor.row+1, anchor.col+1};
      new_points[3] = {anchor.row  , anchor.col+2};
      break;
    case 1:
      new_points[0] = {anchor.row+1, anchor.col+1};
      new_points[1] = {anchor.row+1, anchor.col  };
      new_points[2] = {anchor.row+1, anchor.col-1};
      new_points[3] = {anchor.row+2, anchor.col  };
      break;
    case 2:
      new_points[0] = {anchor.row+1, anchor.col-1};
      new_points[1] = {anchor.row  , anchor.col-1};
      new_points[2] = {anchor.row-1, anchor.col-1};
      new_points[3] = {anchor.row  , anchor.col-2};
      break;
    case 3:
      new_points[0] = {anchor.row-1, anchor.col-1};
      new_points[1] = {anchor.row-1, anchor.col  };
      new_points[2] = {anchor.row-1, anchor.col+1};
      new_points[3] = {anchor.row-2, anchor.col  };
      break;
  }

  rotation += 1;
  rotation %= 4;
  points = new_points;
}


// ZPiece
ZPiece::ZPiece() {
  points = {{0, 4}, {0, 5}, {1, 5}, {1, 6}};
  prev_points = points;
  type = PieceType::Z;
}


void ZPiece::RotateCW() {
  Points new_points{4};
  const Point anchor = points[0];
  switch (rotation) {
    case 0:
      new_points[0] = {anchor.row  , anchor.col+2};
      new_points[1] = {anchor.row+1, anchor.col+2};
      new_points[2] = {anchor.row+1, anchor.col+1};
      new_points[3] = {anchor.row+2, anchor.col+1};
      break;
    case 1:
      new_points[0] = {anchor.row+1, anchor.col-2};
      new_points[1] = {anchor.row+1, anchor.col-1};
      new_points[2] = {anchor.row+2, anchor.col-1};
      new_points[3] = {anchor.row+2, anchor.col  };
      break;
    case 2:
      new_points[0] = {anchor.row-1, anchor.col+1};
      new_points[1] = {anchor.row  , anchor.col+1};
      new_points[2] = {anchor.row  , anchor.col  };
      new_points[3] = {anchor.row+1, anchor.col  };
      break;
    case 3:
      new_points[0] = {anchor.row  , anchor.col-1};
      new_points[1] = {anchor.row  , anchor.col  };
      new_points[2] = {anchor.row+1, anchor.col  };
      new_points[3] = {anchor.row+1, anchor.col+1};
      break;
  }

  rotation += 1;
  rotation %= 4;
  points = new_points;
}


} // namespace Tetris
