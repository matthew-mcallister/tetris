#include <cassert>

#include "tetromino.h"

#include <iostream>

namespace tetris {

const Mask BASE_TETROMINOS[] = {
    // LeftS
    { 0b110'011, 2, 3 },
    // RightS
    { 0b011'110, 2, 3 },
    // LeftL
    { 0b11'01'01, 3, 2 },
    // RightL
    { 0b11'10'10, 3, 2 },
    // Square
    { 0b11'11, 2, 2 },
    // T
    { 0b111'010, 2, 3 },
    // I
    { 0b1'1'1'1, 4, 1 }
};

Tetromino::Tetromino(BaseTetromino base, Color color, Orientation orientation)
    : m_mask{BASE_TETROMINOS[base]}
    , m_color{color}
{
    rotate(orientation);
}

auto Mask::rotate(Orientation orientation) const -> Mask {
    Mask new_mask;
    switch (orientation) {
    case OR_0:
        return *this;
    case OR_90:
        new_mask = { 0, width, height };
        for (int i = 0; i < new_mask.height; i++) {
            for (int j = 0; j < new_mask.width; j++) {
                bool value = get(height - 1 - j, i);
                new_mask.set(i, j, value);
            }
        }
        break;
    case OR_180:
        new_mask = { 0, height, width };
        for (int i = 0; i < new_mask.height; i++) {
            for (int j = 0; j < new_mask.width; j++) {
                bool value = get(height - 1 - i, width - 1 - j);
                new_mask.set(i, j, value);
            }
        }
        break;
    case OR_270:
        new_mask = { 0, width, height };
        for (int i = 0; i < new_mask.height; i++) {
            for (int j = 0; j < new_mask.width; j++) {
                bool value = get(j, width - 1 - i);
                new_mask.set(i, j, value);
            }
        }
        break;
    }

    return new_mask;
}

}
