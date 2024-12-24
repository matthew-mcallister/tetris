#include <algorithm>

#include <rpi-rgb-led-matrix/include/graphics.h>
#include <rpi-rgb-led-matrix/include/led-matrix.h>

using rgb_matrix::RGBMatrix;

enum Color {
    Empty,
    Red,
    Green,
    Blue,
    Yellow
};

rgb_matrix::Color color_to_rgb(Color color) {
    switch (color) {
    case Empty:
        return {0, 0, 0};
    case Red:
        return {255, 0, 0};
    case Green:
        return {0, 255, 0};
    case Blue:
        return {0, 0, 255};
    case Yellow:
        return {255, 188, 0};
    }
    throw 1;
}

Color FIELD[20][10] = {
#define R Red,
#define G Green,
#define B Blue,
#define Y Yellow,
    Y G Y Y R R R R Y Y
    Y G G Y Y B B Y Y G
    Y G B R R B B R B G
    Y B B R R Y R R B G
    G B R Y Y Y R B B G
    G G R R R G G Y Y Y
    B G Y B B G R R Y R
    B B Y B B G B R R R
    B G Y Y R R B Y Y R
    G G G R R B B Y Y R

    Y G Y Y R R R R Y Y
    Y G G Y Y B B Y Y G
    Y G B R R B B R B G
    Y B B R R Y R R B G
    G B R Y Y Y R B B G
    G G R R R G G Y Y Y
    B G Y B B G R R Y R
    B B Y B B G B R R R
    B G Y Y R R B Y Y R
    G G G R R B B Y Y Red
#undef R
#undef G
#undef B
#undef Y
};

class BlockField {
public:
    BlockField() {
        std::fill_n(&m_rows[0][0], 10 * 20, Empty);
    }

    BlockField(const Color* colors) {
        std::copy(colors, colors + 10 * 20, &m_rows[0][0]);
    }

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

private:
    Color m_rows[20][10];
};

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
                m_canvas->SetPixels(3 * i + 1, 3 * j + 1, 3, 3, colors);
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

    while (true) {
        BlockField blocks(&FIELD[0][0]);

        renderer.render(blocks);
        renderer.display();
    }
}
