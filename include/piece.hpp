#ifndef TETRIS_PIECE_HPP_
#define TETRIS_PIECE_HPP_


#include <vector>
#include <utility>

#include "points.hpp"


namespace Tetris {


enum class PieceType {
  I = 0, J, L, O, S, T, Z
};


struct Piece {
  virtual void RotateCW() = 0;
  void Translate(int rows, int cols);

  int rotation = 0;
  Points points;
  Points prev_points;
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
