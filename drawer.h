#pragma once

#include <cstdlib>

// #include "bmp.h"
class BMP;
struct BMPInfoHeader;

struct Point {
    int x = 0;
    int y = 0;
};


inline int Sign(int value) {
    return (0 < value) - (value < 0);
}

inline void SetPixel(BMP& bmp, Point p, BMPColorHeader colour) {
    auto width = bmp.GetWidth();
    if (p.x >= (uint32_t)width || p.y >= (uint32_t)width || p.x < 0 || p.y < 0) {
        throw std::runtime_error("The point is outside the image boundaries!");
    }

    uint32_t channels = bmp.GetBitCount() / 8;
    auto data = bmp.GetData();
    data[channels * (p.y * width + p.x) + 0] = colour.blue_mask;
    data[channels * (p.y * width + p.x) + 1] = colour.green_mask;
    data[channels * (p.y * width + p.x) + 2] = colour.red_mask;
    if (channels == 4) {
        data[channels * (p.y * width + p.x) + 3] = colour.alpha_mask;
    }
}


/**
 * Рисует крутой отрезок (для которого |to.y - from.x| >= |to.x - from.x|).
 */
inline void DrawSteepLine(BMP& bmp, Point from, Point to, BMPColorHeader color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    // assert(delta_y >= delta_x);

    if (from.y > to.y) {  // Крутые отрезки рисуем сверху вниз.
        std::swap(from, to);
    }

    const int step_x = Sign(to.x - from.x);  // Шаг по оси X (-1, 0 или 1).
    const int error_threshold = delta_y + 1;  // Порог ошибки вычисления координаты X.
    const int delta_err = delta_x + 1;  // Шаг накопления ошибки.

    // Когда начальное значение ошибки начинается не с 0, а с delta_err/2,
    // отрезок получается красивее.
    int error = delta_err / 2;

    for (Point p = from; p.y <= to.y; ++p.y) {
        SetPixel(bmp, {p.x, p.y}, color);
        // assert((p.y != to.y) || (p.x == to.x));

        error += delta_err;  // Накапливаем ошибку вычисления координаты X.

        if (error >= error_threshold) {  // Если вышли за пределы текущей координаты X
            p.x += step_x;  // Смещаемся к следующей координате X
            error -= error_threshold;  // Сбрасываем ошибку
        }
    }
}

/**
 * Рисует пологий отрезок (для которого |to.y - from.x| >= |to.y - from.y|).
 */
inline void DrawSlopeLine(BMP& bmp,Point from, Point to, BMPColorHeader color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    // assert(delta_x >= delta_y);

    if (from.x > to.x) {  // Пологие отрезки рисуем слева направо.
        std::swap(from, to);
    }

    // Пологие отрезки рисуются по аналогии с крутыми.

    const int step_y = Sign(to.y - from.y);
    const int error_threshold = delta_x + 1;
    const int delta_err = delta_y + 1;

    int error = delta_err / 2;

    for (Point p = from; p.x <= to.x; ++p.x) {
        SetPixel(bmp, {p.x, p.y}, color);
        // assert((p.x != to.x) || (p.y == to.y));

        error += delta_err;

        if (error >= error_threshold) {
            p.y += step_y;
            error -= error_threshold;
        }
    }
}

// }  // namespace detail

/**
 * Рисует отрезок прямой линии между точками from и to цветом color на изображении Image.
 *
 * Для рисования используется алгоритм Брезенхэма.
 */
inline void DrawLine(BMP& bmp, Point from, Point to, BMPColorHeader color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    if (delta_y > delta_x) {  // Отрезок крутой.
        DrawSteepLine(bmp, from, to, color);
    } else {  // Отрезок пологий.
        DrawSlopeLine(bmp, from, to, color);
    }
}