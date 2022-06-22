#include "piece.hpp"

#include <algorithm>

#include "board.hpp"


namespace Tetris {


void Piece::RotateCCW() {
  RotateCW();
  RotateCW();
  RotateCW();
}


// IPiece
IPiece::IPiece() : Piece{{{1, 3}, {1, 4}, {1, 5}, {1, 6}}} {
  type = PieceType::I;
}


void IPiece::RotateCW() {
  const Point anchor = (*this)[0];
  switch (rotation) {
    case 0:
      (*this)[0] = {anchor.row-1, anchor.col+2};
      (*this)[1] = {anchor.row  , anchor.col+2};
      (*this)[2] = {anchor.row+1, anchor.col+2};
      (*this)[3] = {anchor.row+2, anchor.col+2};
      break;
    case 1:
      (*this)[0] = {anchor.row+2, anchor.col+1};
      (*this)[1] = {anchor.row+2, anchor.col  };
      (*this)[2] = {anchor.row+2, anchor.col-1};
      (*this)[3] = {anchor.row+2, anchor.col-2};
      break;
    case 2:
      (*this)[0] = {anchor.row+1, anchor.col-2};
      (*this)[1] = {anchor.row  , anchor.col-2};
      (*this)[2] = {anchor.row-1, anchor.col-2};
      (*this)[3] = {anchor.row-2, anchor.col-2};
      break;
    case 3:
      (*this)[0] = {anchor.row-2, anchor.col-1};
      (*this)[1] = {anchor.row-2, anchor.col  };
      (*this)[2] = {anchor.row-2, anchor.col+1};
      (*this)[3] = {anchor.row-2, anchor.col+2};
      break;
  }

  rotation += 1;
  rotation %= 4;
}


// JPiece
JPiece::JPiece() : Piece{{{0, 4}, {1, 4}, {1, 5}, {1, 6}}} {
  type = PieceType::J;
}


void JPiece::RotateCW() {
  const Point anchor = (*this)[0];
  switch (rotation) {
    case 0:
      (*this)[0] = {anchor.row  , anchor.col+2};
      (*this)[1] = {anchor.row  , anchor.col+1};
      (*this)[2] = {anchor.row+1, anchor.col+1};
      (*this)[3] = {anchor.row+2, anchor.col+1};
      break;
    case 1:
      (*this)[0] = {anchor.row+2, anchor.col  };
      (*this)[1] = {anchor.row+1, anchor.col  };
      (*this)[2] = {anchor.row+1, anchor.col-1};
      (*this)[3] = {anchor.row+1, anchor.col-2};
      break;
    case 2:
      (*this)[0] = {anchor.row  , anchor.col-2};
      (*this)[1] = {anchor.row  , anchor.col-1};
      (*this)[2] = {anchor.row-1, anchor.col-1};
      (*this)[3] = {anchor.row-2, anchor.col-1};
      break;
    case 3:
      (*this)[0] = {anchor.row-2, anchor.col  };
      (*this)[1] = {anchor.row-1, anchor.col  };
      (*this)[2] = {anchor.row-1, anchor.col+1};
      (*this)[3] = {anchor.row-1, anchor.col+2};
      break;
  }

  rotation += 1;
  rotation %= 4;
}


// LPiece
LPiece::LPiece() : Piece{{{1, 4}, {1, 5}, {1, 6}, {0, 6}}} {
  type = PieceType::L;
}


void LPiece::RotateCW() {
  const Point anchor = (*this)[0];
  switch (rotation) {
    case 0:
      (*this)[0] = {anchor.row-1, anchor.col+1};
      (*this)[1] = {anchor.row  , anchor.col+1};
      (*this)[2] = {anchor.row+1, anchor.col+1};
      (*this)[3] = {anchor.row+1, anchor.col+2};
      break;
    case 1:
      (*this)[0] = {anchor.row+1, anchor.col+1};
      (*this)[1] = {anchor.row+1, anchor.col  };
      (*this)[2] = {anchor.row+1, anchor.col-1};
      (*this)[3] = {anchor.row+2, anchor.col-1};
      break;
    case 2:
      (*this)[0] = {anchor.row+1, anchor.col-1};
      (*this)[1] = {anchor.row  , anchor.col-1};
      (*this)[2] = {anchor.row-1, anchor.col-1};
      (*this)[3] = {anchor.row-1, anchor.col-2};
      break;
    case 3:
      (*this)[0] = {anchor.row-1, anchor.col-1};
      (*this)[1] = {anchor.row-1, anchor.col  };
      (*this)[2] = {anchor.row-1, anchor.col+1};
      (*this)[3] = {anchor.row-2, anchor.col+1};
      break;
  }

  rotation += 1;
  rotation %= 4;
}


// OPiece
OPiece::OPiece() : Piece{{{0, 4}, {0, 5}, {1, 4}, {1, 5}}} {
  type = PieceType::O;
}


void OPiece::RotateCW() {
  return;
}


// SPiece
SPiece::SPiece() : Piece{{{1, 4}, {1, 5}, {0, 5}, {0, 6}}} {
  type = PieceType::S;
}


void SPiece::RotateCW() {
  const Point anchor = (*this)[0];
  switch (rotation) {
    case 0:
      (*this)[0] = {anchor.row-1, anchor.col+1};
      (*this)[1] = {anchor.row  , anchor.col+1};
      (*this)[2] = {anchor.row  , anchor.col+2};
      (*this)[3] = {anchor.row+1, anchor.col+2};
      break;
    case 1:
      (*this)[0] = {anchor.row+2, anchor.col-1};
      (*this)[1] = {anchor.row+2, anchor.col  };
      (*this)[2] = {anchor.row+1, anchor.col  };
      (*this)[3] = {anchor.row+1, anchor.col+1};
      break;
    case 2:
      (*this)[0] = {anchor.row-2, anchor.col  };
      (*this)[1] = {anchor.row-1, anchor.col  };
      (*this)[2] = {anchor.row-1, anchor.col+1};
      (*this)[3] = {anchor.row  , anchor.col+1};
      break;
    case 3:
      (*this)[0] = {anchor.row+1, anchor.col  };
      (*this)[1] = {anchor.row+1, anchor.col+1};
      (*this)[2] = {anchor.row  , anchor.col+1};
      (*this)[3] = {anchor.row  , anchor.col+2};
      break;
  }

  rotation += 1;
  rotation %= 4;
}


// TPiece
TPiece::TPiece() : Piece{{{1, 4}, {1, 5}, {1, 6}, {0, 5}}} {
  type = PieceType::T;
}


void TPiece::RotateCW() {
  const Point anchor = (*this)[0];
  switch (rotation) {
    case 0:
      (*this)[0] = {anchor.row-1, anchor.col+1};
      (*this)[1] = {anchor.row  , anchor.col+1};
      (*this)[2] = {anchor.row+1, anchor.col+1};
      (*this)[3] = {anchor.row  , anchor.col+2};
      break;
    case 1:
      (*this)[0] = {anchor.row+1, anchor.col+1};
      (*this)[1] = {anchor.row+1, anchor.col  };
      (*this)[2] = {anchor.row+1, anchor.col-1};
      (*this)[3] = {anchor.row+2, anchor.col  };
      break;
    case 2:
      (*this)[0] = {anchor.row+1, anchor.col-1};
      (*this)[1] = {anchor.row  , anchor.col-1};
      (*this)[2] = {anchor.row-1, anchor.col-1};
      (*this)[3] = {anchor.row  , anchor.col-2};
      break;
    case 3:
      (*this)[0] = {anchor.row-1, anchor.col-1};
      (*this)[1] = {anchor.row-1, anchor.col  };
      (*this)[2] = {anchor.row-1, anchor.col+1};
      (*this)[3] = {anchor.row-2, anchor.col  };
      break;
  }

  rotation += 1;
  rotation %= 4;
}


// ZPiece
ZPiece::ZPiece() : Piece{{{0, 4}, {0, 5}, {1, 5}, {1, 6}}} {
  type = PieceType::Z;
}


void ZPiece::RotateCW() {
  const Point anchor = (*this)[0];
  switch (rotation) {
    case 0:
      (*this)[0] = {anchor.row  , anchor.col+2};
      (*this)[1] = {anchor.row+1, anchor.col+2};
      (*this)[2] = {anchor.row+1, anchor.col+1};
      (*this)[3] = {anchor.row+2, anchor.col+1};
      break;
    case 1:
      (*this)[0] = {anchor.row+1, anchor.col-2};
      (*this)[1] = {anchor.row+1, anchor.col-1};
      (*this)[2] = {anchor.row+2, anchor.col-1};
      (*this)[3] = {anchor.row+2, anchor.col  };
      break;
    case 2:
      (*this)[0] = {anchor.row-1, anchor.col+1};
      (*this)[1] = {anchor.row  , anchor.col+1};
      (*this)[2] = {anchor.row  , anchor.col  };
      (*this)[3] = {anchor.row+1, anchor.col  };
      break;
    case 3:
      (*this)[0] = {anchor.row  , anchor.col-1};
      (*this)[1] = {anchor.row  , anchor.col  };
      (*this)[2] = {anchor.row+1, anchor.col  };
      (*this)[3] = {anchor.row+1, anchor.col+1};
      break;
  }

  rotation += 1;
  rotation %= 4;
}


} // namespace Tetris
