#include <fstream>
#include <iostream>
#include <stdexcept>

#include "bmp.h"
#include "drawer.h"

constexpr static char WHITE = ' ';
constexpr static char BLACK = '#';

namespace bmp {
BMP::BMP(const std::string file_name) {
    Read(file_name);
}

void BMP::DrawCross() {
    BMPColorHeader cross_colour = {0,0,0};
    Point from_up = {0,0};
    Point to_down = {bmp_info_header_.width-1, bmp_info_header_.height-1};
    Point from_down = {0,bmp_info_header_.height-1};
    Point to_up = {bmp_info_header_.width-1, 0};
    DrawLine(*this, from_up, to_down, cross_colour);
    DrawLine(*this, from_down, to_up, cross_colour);
}

void BMP::Read(const std::string& file_name) {
    std::ifstream inf{ file_name, std::ios_base::binary };
    if (inf) {
        inf.read((char*)&file_header_, sizeof(file_header_));
        if(file_header_.file_type != 0x4D42) {
            throw std::runtime_error("Error! Unrecognized file format.");
        }
        inf.read((char*)&bmp_info_header_, sizeof(bmp_info_header_));

        if(bmp_info_header_.bit_count == 32) {
            if(bmp_info_header_.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                inf.read((char*)&bmp_color_header_, sizeof(bmp_color_header_));
                CheckColorHeader(bmp_color_header_);
            } else {
                std::cerr << "Error! The file \"" << file_name << "\" does not seem to contain bit mask information\n";
                throw std::runtime_error("Error! Unrecognized file format.");
            }
        }

        inf.seekg(file_header_.offset_data, inf.beg);

        if(bmp_info_header_.bit_count == 32) {
            bmp_info_header_.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            file_header_.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        } else {
            bmp_info_header_.size = sizeof(BMPInfoHeader);
            file_header_.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
        }
        file_header_.file_size = file_header_.offset_data;

        if (bmp_info_header_.height < 0) {
            throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
        }

        data_.resize(bmp_info_header_.width * bmp_info_header_.height * bmp_info_header_.bit_count / 8);

        if (bmp_info_header_.width % 4 == 0) {
            inf.read((char*)data_.data(), data_.size());
            file_header_.file_size += static_cast<uint32_t>(data_.size());
        }
        else {
            row_stride_ = bmp_info_header_.width * bmp_info_header_.bit_count / 8;
            uint32_t new_stride = MakeStrideAligned(4);
            std::vector<uint8_t> padding_row(new_stride - row_stride_);

            for (int y = 0; y < bmp_info_header_.height; ++y) {
                inf.read((char*)(data_.data() + row_stride_ * y), row_stride_);
                inf.read((char*)padding_row.data(), padding_row.size());
            }
            file_header_.file_size += static_cast<uint32_t>(data_.size()) + bmp_info_header_.height * static_cast<uint32_t>(padding_row.size());
        }
    }
    else {
        throw std::runtime_error("Unable to open the input image file.");
    }
}

void BMP::WriteInTerminal() {

    uint32_t channels = bmp_info_header_.bit_count / 8;
    for (int y = 0; y < bmp_info_header_.height; ++y) {
        for (int x = 0; x < bmp_info_header_.width; ++x) {
            auto blue =  data_[channels * (y * bmp_info_header_.width + x) + 0];
            auto green = data_[channels * (y * bmp_info_header_.width + x) + 1];
            auto red = data_[channels * (y * bmp_info_header_.width + x) + 2];

            std::cout << ((red == 255 && green == 255 && blue == 255) ? WHITE : BLACK);
        }
        std::cout << std::endl;
    }
}

void BMP::WriteHeaders(std::ofstream &outf) {
    outf.write((const char*)&file_header_, sizeof(file_header_));
    outf.write((const char*)&bmp_info_header_, sizeof(bmp_info_header_));
    if(bmp_info_header_.bit_count == 32) {
        outf.write((const char*)&bmp_color_header_, sizeof(bmp_color_header_));
    }
}

void BMP::WriteHeadersAndData(std::ofstream &outf) {
    WriteHeaders(outf);
    outf.write((const char*)data_.data(), data_.size());
}

uint32_t BMP::MakeStrideAligned(uint32_t align_stride) {
    uint32_t new_stride = row_stride_;
    while (new_stride % align_stride != 0) {
        new_stride++;
    }
    return new_stride;
}

void BMP::CheckColorHeader(BMPColorHeader &bmp_color_header) {
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

void BMP::WriteInFile(const std::string& file_name) {
    std::ofstream outf{ file_name, std::ios_base::binary };
    if (outf) {
        if (bmp_info_header_.bit_count == 32) {
            WriteHeadersAndData(outf);
        }
        else if (bmp_info_header_.bit_count == 24) {
            if (bmp_info_header_.width % 4 == 0) {
                WriteHeadersAndData(outf);
            }
            else {
                uint32_t new_stride = MakeStrideAligned(4);
                std::vector<uint8_t> padding_row(new_stride - row_stride_);

                WriteHeaders(outf);

                for (int y = 0; y < bmp_info_header_.height; ++y) {
                    outf.write((const char*)(data_.data() + row_stride_ * y), row_stride_);
                    outf.write((const char*)padding_row.data(), padding_row.size());
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

void BMP::SetPixel(const Point& point, const BMPColorHeader& colour) {
    auto width = bmp_info_header_.width;
    if (point.x >= width || point.y >= width || point.x < 0 || point.y < 0) {
        throw std::runtime_error("The point is outside the image boundaries!");
    }

    uint32_t channels = bmp_info_header_.bit_count / 8;
    data_[channels * (point.y * width + point.x) + 0] = colour.blue_mask;
    data_[channels * (point.y * width + point.x) + 1] = colour.green_mask;
    data_[channels * (point.y * width + point.x) + 2] = colour.red_mask;
    if (channels == 4) {
        data_[channels * (point.y * width + point.x) + 3] = colour.alpha_mask;
    }
}
} // namespace bmp