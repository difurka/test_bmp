#pragma once

#include <cstdlib>
#include <stdexcept>
#include "bmp.h"

namespace detail {

inline int Sign(int value) {
    return (0 < value) - (value < 0);
}

// inline void SetPixel(BMP& bmp, const Point& p, BMPColorHeader& colour) {
//     auto width = bmp.GetWidth();
//     if (p.x >= (uint32_t)width || p.y >= (uint32_t)width || p.x < 0 || p.y < 0) {
//         throw std::runtime_error("The point is outside the image boundaries!");
//     }

//     uint32_t channels = bmp.GetBitCount() / 8;
//     auto data = bmp.GetData();
//     bmp.GetData()[channels * (p.y * width + p.x) + 0] = colour.blue_mask;
//     bmp.GetData()[channels * (p.y * width + p.x) + 1] = colour.green_mask;
//     bmp.GetData()[channels * (p.y * width + p.x) + 2] = colour.red_mask;
//     if (channels == 4) {
//         bmp.GetData()[channels * (p.y * width + p.x) + 3] = colour.alpha_mask;
//     }
// }


/**
 * Draw steep line (for case |to.y - from.y| >= |to.x - from.x|).
 */
inline void DrawSteepLine(BMP& bmp, Point& from, Point& to, BMPColorHeader& color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    if (from.y > to.y) {
        std::swap(from, to);
    }

    const int step_x = Sign(to.x - from.x);
    const int error_threshold = delta_y + 1;
    const int delta_err = delta_x + 1;

    int error = delta_err / 2;

    for (Point p = from; p.y <= to.y; ++p.y) {
        bmp.SetPixel({p.x, p.y}, color);
        error += delta_err;

        if (error >= error_threshold) {
            p.x += step_x;
            error -= error_threshold;
        }
    }
}

/**
 * Draw slope line (for case  |to.x - from.x| >= |to.y - from.y|).
 */
inline void DrawSlopeLine(BMP& bmp, Point& from, Point& to, BMPColorHeader& color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    if (from.x > to.x) {
        std::swap(from, to);
    }

    const int step_y = Sign(to.y - from.y);
    const int error_threshold = delta_x + 1;
    const int delta_err = delta_y + 1;

    int error = delta_err / 2;

    for (Point p = from; p.x <= to.x; ++p.x) {
        bmp.SetPixel({p.x, p.y}, color);
        error += delta_err;

        if (error >= error_threshold) {
            p.y += step_y;
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
inline void DrawLine(BMP& bmp, Point& from, Point& to, BMPColorHeader& color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    if (delta_y > delta_x) {
        detail::DrawSteepLine(bmp, from, to, color);
    } else {
        detail::DrawSlopeLine(bmp, from, to, color);
    }
}
