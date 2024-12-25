#ifndef TETRIS_GRAPHICS_H
#define TETRIS_GRAPHICS_H

#include <algorithm>
#include <cstdint>

namespace tetris {

class Tetromino;

enum Color {
    Empty,
    Red,
    Green,
    Blue,
    Yellow
};

class BlockField {
public:
    BlockField() {
        std::fill_n(&m_rows[0][0], 10 * 20, Empty);
    }

    BlockField(const Color* colors) {
        std::copy(colors, colors + 10 * 20, &m_rows[0][0]);
    }

    BlockField(const BlockField& other) = default;

    size_t width() const {
        return 10;
    }

    size_t height() const {
        return 20;
    }

    Color* operator[](size_t index) {
        return m_rows[index];
    }

    const Color* operator[](size_t index) const {
        return m_rows[index];
    }

    void paint_tetromino(int x, int y, const Tetromino& tetromino);

private:
    Color m_rows[20][10];
};

}

#endif
