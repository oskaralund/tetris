#pragma once

#include <vector>
#include <utility>


namespace Tetris {


enum class PieceType {
  I = 0, J, L, O, S, T, Z
};


using Point = std::pair<int, int>;
using Points = std::vector<Point>;


} // namespace Tetris
