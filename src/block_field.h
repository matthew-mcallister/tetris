#ifndef TETRIS_BLOCK_FIELD_H
#define TETRIS_BLOCK_FIELD_H

#include <algorithm>
#include <cstdint>

#include "tetromino.h"

namespace tetris {

class BlockField {
public:
    BlockField() {
        reset();
    }

    BlockField(const Color* colors) {
        std::copy(colors, colors + width() * height(), &m_rows[0][0]);
    }

    BlockField(const BlockField& other) = default;

    void reset() {
        std::fill_n(&m_rows[0][0], width() * height(), Empty);
    }

    constexpr size_t width() const {
        return 10;
    }

    constexpr size_t height() const {
        return 20;
    }

    Color* operator[](size_t index) {
        return m_rows[index];
    }

    const Color* operator[](size_t index) const {
        return m_rows[index];
    }

    Mask get_mask(int row, int col, uint8_t h, uint8_t w) const;
    bool tetromino_intersects(int row, int col, const Tetromino& tetromino) const {
        return tetromino.mask() & get_mask(row, col, tetromino.height(), tetromino.width());
    }

    void paint_tetromino(int row, int col, const Tetromino& tetromino);

private:
    Color m_rows[20][10];
};

}

#endif
