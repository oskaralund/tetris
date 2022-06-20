#include "board.hpp"


namespace Tetris {


void Board::Clear() {
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col <cols; ++col) {
      matrix[row][col] = 0;
    }
  }
}


} // namespace Tetris
