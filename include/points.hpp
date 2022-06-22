#ifndef TETRIS_POINTS_HPP_
#define TETRIS_POINTS_HPP_


#include <vector>

#include "point.hpp"


namespace Tetris {


// Represents a set of points on the playfield. Note that we can translate the
// points conveniently, e.g.: points + {1,1}
struct Points : std::vector<Point> {
  using std::vector<Point>::vector;

  Points operator+(const int(&rhs)[2]) {
    Points sum = *this;
    for (auto& p : sum) {
      p += rhs;
    }
    return sum;
  }

  Points& operator+=(const int(&rhs)[2]) {
    *this = *this + rhs;
    return *this;
  }
};


} // namespace Tetris


#endif
