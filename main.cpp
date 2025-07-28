#include <iostream>
#include "bmp.h"
#include "drawer.h"


int main() {
    std::string input_file_path, output_file_path;
    std::cout << "Enter input BMP file name: ";
    std::getline(std::cin,  input_file_path);
	BMP bmp(input_file_path);
    bmp.DrawCross();
    bmp.WriteInTerminal();
    std::cout << "Enter output BMP file name: ";
    std::getline(std::cin, output_file_path);
	bmp.WriteInFile(output_file_path);
}