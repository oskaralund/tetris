#pragma once

#include <vector>
#include <utility>

#include "tetris.hpp"


namespace Tetris {

class Board;


struct Piece {
  virtual void RotateCW() = 0;
  bool ValidPosition(Points) const;
  void Step();
  void UndoStep();
  void Left();
  void Right();
  void Translate(int rows, int cols);

  int rotation = 0;
  Points points;
  Points prev_points;
  Board* board;
  PieceType type;
};


struct IPiece : public Piece {
  IPiece();
  void RotateCW() override;
};


struct LPiece : public Piece {
  LPiece();
  void RotateCW() override;
};


struct JPiece : public Piece {
  JPiece();
  void RotateCW() override;
};


struct OPiece : public Piece {
  OPiece();
  void RotateCW() override;
};


struct SPiece : public Piece {
  SPiece();
  void RotateCW() override;
};


struct TPiece : public Piece {
  TPiece();
  void RotateCW() override;
};


struct ZPiece : public Piece {
  ZPiece();
  void RotateCW() override;
};


} // namespace Tetris
