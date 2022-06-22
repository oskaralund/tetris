#ifndef TETRIS_POINT_HPP_
#define TETRIS_POINT_HPP_


namespace Tetris {


struct Point {
  int row = 0;
  int col = 0;

  Point operator+(const Point& rhs) { return { row+rhs.row, col+rhs.col }; }
};


}
#endif
