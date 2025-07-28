#pragma once
#include <cstdint>
// #include <fstream>
// #include <iostream>
#include <vector>
// #include <stdexcept>
#include <string>

struct Point {
    int x = 0;
    int y = 0;
};


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
    BMPFileHeader file_header_;
    BMPInfoHeader bmp_info_header_;
    BMPColorHeader bmp_color_header_;
    std::vector<uint8_t> data_;

    uint32_t row_stride_{ 0 };

    void WriteHeaders(std::ofstream &of);
    void WriteHeadersAndData(std::ofstream &of);

    // Add 1 to the row_stride until it is divisible with align_stride
    uint32_t MakeStrideAligned(uint32_t align_stride);
    void CheckColorHeader(BMPColorHeader &bmp_color_header);
 public:
    BMP(const std::string fname);
    void DrawCross();
    void Read(const std::string& fname);
    void WriteInTerminal();
    void WriteInFile(const std::string& fname);
    int32_t GetWidth() const;
    uint16_t GetBitCount() const;
    std::vector<uint8_t> GetData();
};


// namespace detail {

