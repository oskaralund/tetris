#ifndef TETRIS_PIECE_HPP_
#define TETRIS_PIECE_HPP_


#include <vector>
#include <utility>

#include "points.hpp"


namespace Tetris {


enum class PieceType {
  I, J, L, O, S, T, Z
};


// Base class for tetris pieces. We inherit from this and implement the
// RotateCW() function to define a specific piece. Note that this class
// inherits from Points, so we can do simple translation, e.g.: piece + {1,0}
// to move the piece one row down.
struct Piece : Points {
  using Points::Points;

  virtual void RotateCW() = 0;
  void RotateCCW();

  int rotation = 0;
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


#endif
