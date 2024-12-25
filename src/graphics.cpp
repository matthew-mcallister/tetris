#include <cstdint>

#include "graphics.h"
#include "tetromino.h"

namespace tetris {

void BlockField::paint_tetromino(int row, int col, const Tetromino& tetromino) {
    for (int i = 0; i < tetromino.height(); i++) {
        int dst_row = row + i;
        if (dst_row < 0 || dst_row >= height()) continue;

        for (int j = 0; j < tetromino.width(); j++) {
            int dst_col = col + j;
            if (dst_col < 0 || dst_col >= width()) continue;
            if (tetromino.mask().get(i, j)) {
                m_rows[dst_row][dst_col] = tetromino.color();
            }
        }
    }
}

}
