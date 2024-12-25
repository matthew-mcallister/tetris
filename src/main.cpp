#include <algorithm>
#include <cstdint>
#include <random>

#include <rpi-rgb-led-matrix/include/graphics.h>
#include <rpi-rgb-led-matrix/include/led-matrix.h>

#include "block_field.h"
#include "graphics.h"
#include "tetromino.h"

using rgb_matrix::RGBMatrix;

namespace tetris {

rgb_matrix::Color color_to_rgb(Color color) {
    switch (color) {
    case Empty:
        return {0, 0, 0};
    case Red:
        return {96, 0, 0};
    case Green:
        return {0, 72, 2};
    case Blue:
        return {0, 0, 96};
    case Yellow:
        return {96, 72, 0};
    }
    throw 1;
}

class Renderer {
public:
    Renderer(RGBMatrix* matrix) : m_matrix{matrix} {
        m_canvas = m_matrix->CreateFrameCanvas();
    }

    ~Renderer() {
        delete m_matrix;
    }

    void render(const BlockField& field) {
        m_canvas->Clear();
        for (int i = 0; i < field.height(); i++) {
            for (int j = 0; j < field.width(); j++) {
                const Color color = field[i][j];
                if (color == Empty) continue;
                const rgb_matrix::Color rgb = color_to_rgb(color);
                rgb_matrix::Color colors[9];
                std::fill_n(colors, 9, rgb);
                m_canvas->SetPixels(3 * i + 3, 3 * (field.width() - 1 - j) + 1, 3, 3, colors);
            }
        }
    }

    void simple_render() {
        m_canvas->Clear();
        const rgb_matrix::Color color{255, 255, 0};
        rgb_matrix::Color colors[100];
        std::fill_n(colors, 100, color);
        m_canvas->SetPixels(0, 0, 10, 10, colors);
    }

    void display() {
        m_canvas = m_matrix->SwapOnVSync(m_canvas);
    }

private:
    RGBMatrix* m_matrix;
    rgb_matrix::FrameCanvas* m_canvas;
};

class Game {
public:
    Game()
        : m_fall_row{0}
        , m_fall_col{0}
        , m_fall{LeftS, Red, OR_0}
    {
        generate_tetromino();
    }

    void update(uint64_t frame) {
        if (frame % m_inverse_blocks_per_frame == 0) {
            fall();
        }
    }

    void render(Renderer& renderer) const {
        BlockField composite(m_blocks);
        composite.paint_tetromino(m_fall_row, m_fall_col, m_fall);
        renderer.render(composite);
    }

private:
    // Fall rate
    uint64_t m_inverse_blocks_per_frame = 10;

    BlockField m_blocks;
    Tetromino m_fall;
    int m_fall_row, m_fall_col;
    std::minstd_rand m_rng;

    void fall() {
        if (m_blocks.tetromino_intersects(m_fall_row + 1, m_fall_col, m_fall)) {
            m_blocks.paint_tetromino(m_fall_row, m_fall_col, m_fall);
            block_placed();
        } else {
            m_fall_row++;
        }
    }

    void block_placed() {
        // TODO: Clear rows
        generate_tetromino();
    }
    
    void generate_tetromino() {
        const auto which = static_cast<BaseTetromino>(m_rng() % BASE_TETROMINO_MAX);
        const auto color = static_cast<Color>(Red + m_rng() % (COLOR_MAX - Red));
        const auto orientation = static_cast<Orientation>(m_rng() % ORIENTATION_MAX);
        m_fall = Tetromino{which, color, orientation};

        const auto max_col = m_blocks.width() - m_fall.width();
        m_fall_row = 0;
        m_fall_col = m_rng() % max_col;

        if (m_blocks.tetromino_intersects(m_fall_row, m_fall_col, m_fall)) {
            m_blocks.reset();
        }
    }
};

int main() {
	rgb_matrix::RuntimeOptions runtime_options;
	runtime_options.gpio_slowdown = 3;
	RGBMatrix::Options matrix_options;
	matrix_options.rows = 32;
	matrix_options.cols = 64;
    matrix_options.hardware_mapping = "adafruit-hat";
	RGBMatrix* matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_options);
    if (!matrix) throw 1;

    Renderer renderer(matrix);

    Game game;
    uint64_t raw_frame = 0;
    const uint64_t refresh_ratio = 3;
    while (true) {
        if (raw_frame % refresh_ratio == 0) {
            const uint64_t frame = raw_frame / refresh_ratio;
            game.update(frame);
        }

        game.render(renderer);
        renderer.display();

        raw_frame++;
    }

    return 0;
}

}

int main() {
    return tetris::main();
}
