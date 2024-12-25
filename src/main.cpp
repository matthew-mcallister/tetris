#include <algorithm>
#include <cstdint>

#include <rpi-rgb-led-matrix/include/graphics.h>
#include <rpi-rgb-led-matrix/include/led-matrix.h>

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

    uint64_t raw_frame = 0;
    const uint64_t refresh_ratio = 3;
    while (true) {
        const uint64_t frame = raw_frame / refresh_ratio;

        BlockField blocks;

        Orientation orientation = static_cast<Orientation>((frame / 60) % ORIENTATION_MAX);
        blocks.paint_tetromino(0, 0, {LeftS, Red, orientation});
        blocks.paint_tetromino(0, 3, {RightS, Green, orientation});
        blocks.paint_tetromino(0, 6, {LeftL, Blue, orientation});
        blocks.paint_tetromino(3, 0, {RightL, Yellow, orientation});
        blocks.paint_tetromino(3, 3, {Square, Red, orientation});
        blocks.paint_tetromino(3, 6, {T, Green, orientation});
        blocks.paint_tetromino(6, 0, {I, Blue, orientation});

        renderer.render(blocks);
        renderer.display();

        raw_frame++;
    }

    return 0;
}

}

int main() {
    return tetris::main();
}
