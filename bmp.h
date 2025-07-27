#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>


#include <drawer.h>

constexpr static char WHITE = '#';
constexpr static char BLACK = ' ';

// struct Point {
//     int x = 0;
//     int y = 0;
// };


#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 };
    uint32_t file_size{ 0 };
    uint16_t reserved1{ 0 };
    uint16_t reserved2{ 0 };
    uint32_t offset_data{ 0 };
};

struct BMPInfoHeader {
    uint32_t size{ 0 };
    int32_t width{ 0 };
    int32_t height{ 0 };
    uint16_t planes{ 1 };
    uint16_t bit_count{ 0 };
    uint32_t compression{ 0 };
    uint32_t size_image{ 0 };
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };
    uint32_t colors_important{ 0 };
};

struct BMPColorHeader {
    uint32_t red_mask{ 0x00ff0000 };
    uint32_t green_mask{ 0x0000ff00 };
    uint32_t blue_mask{ 0x000000ff };
    uint32_t alpha_mask{ 0xff000000 };
    uint32_t color_space_type{ 0x73524742 };
    uint32_t unused[16]{ 0 };
};

#pragma pack(pop)

class BMP {
 private:
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    BMPColorHeader bmp_color_header;
    std::vector<uint8_t> data;
 public:
    BMP(const std::string fname) {
        read(fname);
    }
    
// int Sign(int value) {
//     return (0 < value) - (value < 0);
// }

// void SetPixel(Point p, BMPColorHeader colour) {
//     auto width = GetWidth();
//     if (p.x >= (uint32_t)width || p.y >= (uint32_t)width || p.x < 0 || p.y < 0) {
//         throw std::runtime_error("The point is outside the image boundaries!");
//     }

//     uint32_t channels = GetBitCount() / 8;
//     // auto data = GetData();
//     data[channels * (p.y * width + p.x) + 0] = colour.blue_mask;
//     data[channels * (p.y * width + p.x) + 1] = colour.green_mask;
//     data[channels * (p.y * width + p.x) + 2] = colour.red_mask;
//     if (channels == 4) {
//         data[channels * (p.y * width + p.x) + 3] = colour.alpha_mask;
//     }
// }


// /**
//  * Рисует крутой отрезок (для которого |to.y - from.x| >= |to.x - from.x|).
//  */
// void DrawSteepLine(Point from, Point to, BMPColorHeader color) {
//     const int delta_x = std::abs(to.x - from.x);
//     const int delta_y = std::abs(to.y - from.y);

//     // assert(delta_y >= delta_x);

//     if (from.y > to.y) {  // Крутые отрезки рисуем сверху вниз.
//         std::swap(from, to);
//     }

//     const int step_x = Sign(to.x - from.x);  // Шаг по оси X (-1, 0 или 1).
//     const int error_threshold = delta_y + 1;  // Порог ошибки вычисления координаты X.
//     const int delta_err = delta_x + 1;  // Шаг накопления ошибки.

//     // Когда начальное значение ошибки начинается не с 0, а с delta_err/2,
//     // отрезок получается красивее.
//     int error = delta_err / 2;

//     for (Point p = from; p.y <= to.y; ++p.y) {
//         SetPixel({p.x, p.y}, color);
//         // assert((p.y != to.y) || (p.x == to.x));

//         error += delta_err;  // Накапливаем ошибку вычисления координаты X.

//         if (error >= error_threshold) {  // Если вышли за пределы текущей координаты X
//             p.x += step_x;  // Смещаемся к следующей координате X
//             error -= error_threshold;  // Сбрасываем ошибку
//         }
//     }
// }

// /**
//  * Рисует пологий отрезок (для которого |to.y - from.x| >= |to.y - from.y|).
//  */
// void DrawSlopeLine(Point from, Point to, BMPColorHeader color) {
//     const int delta_x = std::abs(to.x - from.x);
//     const int delta_y = std::abs(to.y - from.y);

//     // assert(delta_x >= delta_y);

//     if (from.x > to.x) {  // Пологие отрезки рисуем слева направо.
//         std::swap(from, to);
//     }

//     // Пологие отрезки рисуются по аналогии с крутыми.

//     const int step_y = Sign(to.y - from.y);
//     const int error_threshold = delta_x + 1;
//     const int delta_err = delta_y + 1;

//     int error = delta_err / 2;

//     for (Point p = from; p.x <= to.x; ++p.x) {
//         SetPixel({p.x, p.y}, color);
//         // assert((p.x != to.x) || (p.y == to.y));

//         error += delta_err;

//         if (error >= error_threshold) {
//             p.y += step_y;
//             error -= error_threshold;
//         }
//     }
// }

// // }  // namespace detail

// /**
//  * Рисует отрезок прямой линии между точками from и to цветом color на изображении Image.
//  *
//  * Для рисования используется алгоритм Брезенхэма.
//  */
// void DrawLine(Point from, Point to, BMPColorHeader color) {
//     const int delta_x = std::abs(to.x - from.x);
//     const int delta_y = std::abs(to.y - from.y);

//     if (delta_y > delta_x) {  // Отрезок крутой.
//         DrawSteepLine(from, to, color);
//     } else {  // Отрезок пологий.
//         DrawSlopeLine(from, to, color);
//     }
// }

    void DrawCross() {
        BMPColorHeader cross_colour = {255,255, 255};
        Point from = {0,0};
        Point to = {bmp_info_header.width-1, bmp_info_header.height-1};
        Point from1 = {0,bmp_info_header.height-1};
        Point to1 = {bmp_info_header.width-1, 0};
        DrawLine(*this, from, to, cross_colour);
        DrawLine(*this, from1, to1, cross_colour);
    }

    void read(const std::string& fname) {
        std::ifstream inp{ fname, std::ios_base::binary };
        if (inp) {
            inp.read((char*)&file_header, sizeof(file_header));
            if(file_header.file_type != 0x4D42) {
                throw std::runtime_error("Error! Unrecognized file format.");
            }
            inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));

            // The BMPColorHeader is used only for transparent images
            if(bmp_info_header.bit_count == 32) {
                // Check if the file has bit mask color information
                if(bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                    inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));
                    // Check if the pixel data is stored as BGRA and if the color space type is sRGB
                    check_color_header(bmp_color_header);
                } else {
                    std::cerr << "Error! The file \"" << fname << "\" does not seem to contain bit mask information\n";
                    throw std::runtime_error("Error! Unrecognized file format.");
                }
            }

            // Jump to the pixel data location
            inp.seekg(file_header.offset_data, inp.beg);

            // Adjust the header fields for output.
            // Some editors will put extra info in the image file, we only save the headers and the data.
            if(bmp_info_header.bit_count == 32) {
                bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
                file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            } else {
                bmp_info_header.size = sizeof(BMPInfoHeader);
                file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
            }
            file_header.file_size = file_header.offset_data;

            if (bmp_info_header.height < 0) {
                throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
            }

            data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);

            // Here we check if we need to take into account row padding
            if (bmp_info_header.width % 4 == 0) {
                inp.read((char*)data.data(), data.size());
                file_header.file_size += static_cast<uint32_t>(data.size());
            }
            else {
                row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
                uint32_t new_stride = make_stride_aligned(4);
                std::vector<uint8_t> padding_row(new_stride - row_stride);

                for (int y = 0; y < bmp_info_header.height; ++y) {
                    inp.read((char*)(data.data() + row_stride * y), row_stride);
                    inp.read((char*)padding_row.data(), padding_row.size());
                }
                file_header.file_size += static_cast<uint32_t>(data.size()) + bmp_info_header.height * static_cast<uint32_t>(padding_row.size());
            }
        }
        else {
            throw std::runtime_error("Unable to open the input image file.");
        }
    }

        // Получение индекса пикселя
    int getPixelIndex(int x, int y) const {

        return (y * row_stride) + (x * (bmp_info_header.bit_count / 8));
    }
    void write_in_terminal() {

        uint32_t channels = GetBitCount() / 8;


        for (int y = 0; y < bmp_info_header.height; ++y) {
            for (int x = 0; x < bmp_info_header.width; ++x) {

                
                auto blue =  data[channels * (y * bmp_info_header.width + x) + 0];
                auto green = data[channels * (y * bmp_info_header.width + x) + 1];
                auto red = data[channels * (y * bmp_info_header.width + x) + 2];

                std::cout << ((red == 255 && green == 255 && blue == 255) ? WHITE : BLACK);
            }
            std::cout << std::endl;
        }
    }

    void write_in_file(const std::string& fname) {
        std::ofstream of{ fname, std::ios_base::binary };
        if (of) {
            if (bmp_info_header.bit_count == 32) {
                write_headers_and_data(of);
            }
            else if (bmp_info_header.bit_count == 24) {
                if (bmp_info_header.width % 4 == 0) {
                    write_headers_and_data(of);
                }
                else {
                    uint32_t new_stride = make_stride_aligned(4);
                    std::vector<uint8_t> padding_row(new_stride - row_stride);

                    write_headers(of);

                    for (int y = 0; y < bmp_info_header.height; ++y) {
                        of.write((const char*)(data.data() + row_stride * y), row_stride);
                        of.write((const char*)padding_row.data(), padding_row.size());
                    }
                }
            }
            else {
                throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
            }
        }
        else {
            throw std::runtime_error("Unable to open the output image file.");
        }
    }



    int32_t GetWidth() const {
        return bmp_info_header.width;
    }

    uint16_t GetBitCount() const {
        return bmp_info_header.bit_count;
    }

    std::vector<uint8_t> GetData() {
        return data;
    }

private:
    uint32_t row_stride{ 0 };

    void write_headers(std::ofstream &of) {
        of.write((const char*)&file_header, sizeof(file_header));
        of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
        if(bmp_info_header.bit_count == 32) {
            of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
        }
    }

    void write_headers_and_data(std::ofstream &of) {
        write_headers(of);
        of.write((const char*)data.data(), data.size());
    }

    // Add 1 to the row_stride until it is divisible with align_stride
    uint32_t make_stride_aligned(uint32_t align_stride) {
        uint32_t new_stride = row_stride;
        while (new_stride % align_stride != 0) {
            new_stride++;
        }
        return new_stride;
    }

    // Check if the pixel data is stored as BGRA and if the color space type is sRGB
    void check_color_header(BMPColorHeader &bmp_color_header) {
        BMPColorHeader expected_color_header;
        if(expected_color_header.red_mask != bmp_color_header.red_mask ||
            expected_color_header.blue_mask != bmp_color_header.blue_mask ||
            expected_color_header.green_mask != bmp_color_header.green_mask ||
            expected_color_header.alpha_mask != bmp_color_header.alpha_mask) {
            throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
        }
        if(expected_color_header.color_space_type != bmp_color_header.color_space_type) {
            throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
        }
    }
};


// namespace detail {

