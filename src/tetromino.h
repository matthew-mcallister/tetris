#ifndef TETRIS_TETROMINO_H
#define TETRIS_TETROMINO_H

#include <cassert>
#include <cstdint>

#include "graphics.h"

namespace tetris {

enum BaseTetromino {
    // xx.
    // .xx
    LeftS,
    // .xx
    // xx.
    RightS,
    // .x
    // .x
    // xx
    LeftL,
    // x.
    // x.
    // xx
    RightL,
    // xx
    // xx
    Square,
    // .x.
    // xxx
    T,
    // x
    // x
    // x
    // x
    I,
    BASE_TETROMINO_MAX
};

enum Orientation {
    OR_0,
    OR_90,
    OR_180,
    OR_270,
    ORIENTATION_MAX
};

/**
 * A rectangular bitmask that carries up to 16 bits. Equivalent to a boolean
 * matrix.
 */
struct Mask {
    uint16_t bits;
    uint8_t height;
    uint8_t width;

    bool get(unsigned int row, unsigned int col) const {
        assert(row < height && col < width);
        int shift = row * width + col;
        return bits & (1 << shift);
    }

    void set(unsigned int row, unsigned int col, bool value) {
        assert(row < height && col < width);
        int shift = row * width + col;
        uint16_t mask = 1 << shift;
        if (value) {
            bits |= mask;
        } else {
            bits &= ~mask;
        }
    }

    bool operator&(const Mask& other) const {
        assert(height == other.height && width == other.width);
        return bits & other.bits;
    }

    auto rotate(Orientation orientation) const -> Mask;
};

class Tetromino {
public:
    Tetromino(BaseTetromino base, Color color, Orientation orientation = OR_0);

    uint8_t width() const { return m_mask.width; }
    uint8_t height() const { return m_mask.height; }
    const Mask& mask() const { return m_mask; }
    Color color() const { return m_color; }

    void rotate(Orientation orientation) {
        m_mask = m_mask.rotate(orientation);
    }

private:
    Mask m_mask;
    Color m_color;
};

}

#endif
