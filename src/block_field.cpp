#include <cstdint>

#include "block_field.h"
#include "tetromino.h"

namespace tetris {

void BlockField::paint_tetromino(const int row, const int col, const Tetromino& tetromino) {
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

Mask BlockField::get_mask(const int row, const int col, const uint8_t h, const uint8_t w) const {
    Mask mask{0, h, w};
    for (int i = 0; i < h; i++) {
        const int cur_row = row + i;
        for (int j = 0; j < w; j++) {
            const int cur_col = col + j;
            bool value;
            if (cur_row < 0 || cur_row >= height() || cur_col < 0 || cur_col >= width()) {
                value = true;
            } else {
                value = m_rows[cur_row][cur_col];
            }
            mask.set(i, j, value);
        }
    }
    return mask;
}
    
}
