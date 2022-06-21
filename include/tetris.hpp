#ifndef TETRIS_TETRIS_HPP_
#define TETRIS_TETRIS_HPP_


#include <vector>
#include <utility>


namespace Tetris {


enum class PieceType {
  I = 0, J, L, O, S, T, Z
};


using Point = std::pair<int, int>;
using Points = std::vector<Point>;


} // namespace Tetris


#endif
