#ifndef TETRIS_POINTS_HPP_
#define TETRIS_POINTS_HPP_


#include <vector>

#include "point.hpp"


namespace Tetris {


struct Points : std::vector<Point> {
  using std::vector<Point>::vector;
  Points operator+(const Point& rhs) {
    Points sum = *this;
    for (auto& p : sum) { p = p + rhs; }
    return sum;
  }
};


} // namespace Tetris


#endif
