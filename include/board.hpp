#pragma once

#include <vector>

namespace Tetris {


struct Board {
  void Clear();

  static const int rows = 24;
  static const int cols = 10;
  int matrix[rows][cols];
};


} // namespace Tetris
