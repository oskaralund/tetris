#ifndef TETRIS_POINT_HPP_
#define TETRIS_POINT_HPP_


namespace Tetris {


// Represents a point on the playfield. Note that we can translate the point
// conveniently, e.g.: point + {1,1}
struct Point {
  int row = 0;
  int col = 0;

  Point operator+(const int (&rhs)[2]) {
    return { row+rhs[0], col+rhs[1] };
  }

  Point& operator+=(const int (&rhs)[2]) {
    *this = *this + rhs;
    return *this;
  }
};


}
#endif
