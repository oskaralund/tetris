#pragma once

#include <vector>
#include <utility>

#include "tetris.hpp"


namespace Tetris {

class Board;


struct Piece {
  Piece(Board*);
  virtual void RotateCW() = 0;
  bool ValidPosition(Points) const;
  void Step();
  void UndoStep();
  void Left();
  void Right();

  int rotation = 0;
  Points points;
  Points prev_points;
  Board* board;
  PieceType type;
};


struct IPiece : public Piece {
  IPiece(Board* board);
  void RotateCW() override;
};


struct LPiece : public Piece {
  LPiece(Board* board);
  void RotateCW() override;
};


struct JPiece : public Piece {
  JPiece(Board* board);
  void RotateCW() override;
};


struct OPiece : public Piece {
  OPiece(Board* board);
  void RotateCW() override;
};


struct SPiece : public Piece {
  SPiece(Board* board);
  void RotateCW() override;
};


struct TPiece : public Piece {
  TPiece(Board* board);
  void RotateCW() override;
};


struct ZPiece : public Piece {
  ZPiece(Board* board);
  void RotateCW() override;
};


} // namespace Tetris
