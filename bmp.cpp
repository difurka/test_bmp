// #include <cstdint>
#include <fstream>
#include <iostream>
// #include <vector>
#include <stdexcept>
// #include <string>

#include "bmp.h"
#include "drawer.h"

constexpr static char WHITE = '#';
constexpr static char BLACK = ' ';



    BMP::BMP(const std::string fname) {
        Read(fname);
    }
    

    void BMP::DrawCross() {
        BMPColorHeader cross_colour = {255, 255, 255};
        Point from = {0,0};
        Point to = {bmp_info_header_.width-1, bmp_info_header_.height-1};
        Point from1 = {0,bmp_info_header_.height-1};
        Point to1 = {bmp_info_header_.width-1, 0};
        DrawLine(*this, from, to, cross_colour);
        DrawLine(*this, from1, to1, cross_colour);
    }

    void BMP::Read(const std::string& fname) {
        std::ifstream inp{ fname, std::ios_base::binary };
        if (inp) {
            inp.read((char*)&file_header_, sizeof(file_header_));
            if(file_header_.file_type != 0x4D42) {
                throw std::runtime_error("Error! Unrecognized file format.");
            }
            inp.read((char*)&bmp_info_header_, sizeof(bmp_info_header_));

            // The BMPColorHeader is used only for transparent images
            if(bmp_info_header_.bit_count == 32) {
                // Check if the file has bit mask color information
                if(bmp_info_header_.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                    inp.read((char*)&bmp_color_header_, sizeof(bmp_color_header_));
                    // Check if the pixel data is stored as BGRA and if the color space type is sRGB
                    CheckColorHeader(bmp_color_header_);
                } else {
                    std::cerr << "Error! The file \"" << fname << "\" does not seem to contain bit mask information\n";
                    throw std::runtime_error("Error! Unrecognized file format.");
                }
            }

            // Jump to the pixel data location
            inp.seekg(file_header_.offset_data, inp.beg);

            // Adjust the header fields for output.
            // Some editors will put extra info in the image file, we only save the headers and the data.
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

            // Here we check if we need to take into account row padding
            if (bmp_info_header_.width % 4 == 0) {
                inp.read((char*)data_.data(), data_.size());
                file_header_.file_size += static_cast<uint32_t>(data_.size());
            }
            else {
                row_stride_ = bmp_info_header_.width * bmp_info_header_.bit_count / 8;
                uint32_t new_stride = MakeStrideAligned(4);
                std::vector<uint8_t> padding_row(new_stride - row_stride_);

                for (int y = 0; y < bmp_info_header_.height; ++y) {
                    inp.read((char*)(data_.data() + row_stride_ * y), row_stride_);
                    inp.read((char*)padding_row.data(), padding_row.size());
                }
                file_header_.file_size += static_cast<uint32_t>(data_.size()) + bmp_info_header_.height * static_cast<uint32_t>(padding_row.size());
            }
        }
        else {
            throw std::runtime_error("Unable to open the input image file.");
        }
    }

    void BMP::WriteInTerminal() {

        uint32_t channels = GetBitCount() / 8;


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


    void BMP::WriteHeaders(std::ofstream &of) {
        of.write((const char*)&file_header_, sizeof(file_header_));
        of.write((const char*)&bmp_info_header_, sizeof(bmp_info_header_));
        if(bmp_info_header_.bit_count == 32) {
            of.write((const char*)&bmp_color_header_, sizeof(bmp_color_header_));
        }
    }

    void BMP::WriteHeadersAndData(std::ofstream &of) {
        WriteHeaders(of);
        of.write((const char*)data_.data(), data_.size());
    }


    uint32_t BMP::MakeStrideAligned(uint32_t align_stride) {
        uint32_t new_stride = row_stride_;
        while (new_stride % align_stride != 0) {
            new_stride++;
        }
        return new_stride;
    }

    // Check if the pixel data is stored as BGRA and if the color space type is sRGB
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



    void BMP::WriteInFile(const std::string& fname) {
        std::ofstream of{ fname, std::ios_base::binary };
        if (of) {
            if (bmp_info_header_.bit_count == 32) {
                WriteHeadersAndData(of);
            }
            else if (bmp_info_header_.bit_count == 24) {
                if (bmp_info_header_.width % 4 == 0) {
                    WriteHeadersAndData(of);
                }
                else {
                    uint32_t new_stride = MakeStrideAligned(4);
                    std::vector<uint8_t> padding_row(new_stride - row_stride_);

                    WriteHeaders(of);

                    for (int y = 0; y < bmp_info_header_.height; ++y) {
                        of.write((const char*)(data_.data() + row_stride_ * y), row_stride_);
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

    int32_t BMP::GetWidth() const {
        return bmp_info_header_.width;
    }

    uint16_t BMP::GetBitCount() const {
        return bmp_info_header_.bit_count;
    }

    std::vector<uint8_t> BMP::GetData() {
        return data_;
    }
