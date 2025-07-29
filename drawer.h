#pragma once

#include <stdexcept>
#include "bmp.h"

namespace detail {

inline int Sign(int value) {
    return (0 < value) - (value < 0);
}

/**
 * Draw steep line (for case |to.y - from.y| >= |to.x - from.x|).
 */
inline void DrawSteepLine(bmp::BMP& bmp, Point& from, Point& to, bmp::BMPColorHeader& color) {
    const int delta_x = std::abs(static_cast<int>(to.x - from.x));
    const int delta_y = std::abs(static_cast<int>(to.y - from.y));

    if (from.y > to.y) {
        std::swap(from, to);
    }

    const int step_x = Sign(to.x - from.x);
    const int error_threshold = delta_y + 1;
    const int delta_err = delta_x + 1;

    int error = delta_err / 2;

    for (Point point = from; point.y <= to.y; ++point.y) {
        bmp.SetPixel({point.x, point.y}, color);
        error += delta_err;

        if (error >= error_threshold) {
            point.x += step_x;
            error -= error_threshold;
        }
    }
}

/**
 * Draw slope line (for case  |to.x - from.x| >= |to.y - from.y|).
 */
inline void DrawSlopeLine(bmp::BMP& bmp, Point& from, Point& to, bmp::BMPColorHeader& color) {
    const int delta_x = std::abs(static_cast<int>(to.x - from.x));
    const int delta_y = std::abs(static_cast<int>(to.y - from.y));

    if (from.x > to.x) {
        std::swap(from, to);
    }

    const int step_y = Sign(to.y - from.y);
    const int error_threshold = delta_x + 1;
    const int delta_err = delta_y + 1;

    int error = delta_err / 2;

    for (Point point = from; point.x <= to.x; ++point.x) {
        bmp.SetPixel({point.x, point.y}, color);
        error += delta_err;

        if (error >= error_threshold) {
            point.y += step_y;
            error -= error_threshold;
        }
    }
}

}  // namespace detail

/**
 * Draw line between points from and to with color
 *
 * Using Bresenham's line algorithm
 */
inline void DrawLine(bmp::BMP& bmp, Point& from, Point& to, bmp::BMPColorHeader& color) {
    const int delta_x = std::abs(static_cast<int>(to.x - from.x));
    const int delta_y = std::abs(static_cast<int>(to.y - from.y));

    if (delta_y > delta_x) {
        detail::DrawSteepLine(bmp, from, to, color);
    } else {
        detail::DrawSlopeLine(bmp, from, to, color);
    }
}
