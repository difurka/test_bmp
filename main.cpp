#include <iostream>
#include "bmp.h"
#include "drawer.h"


int main() {
    std::string input_file_path, output_file_path;
    std::string separate_line = "----------------------------\n";

    std::cout << "Enter input BMP file name: ";
    std::getline(std::cin,  input_file_path);

	bmp::BMP bmp(input_file_path);
    std::cout << separate_line;
    bmp.WriteInTerminal();
    std::cout << separate_line;

    std::cout << "\n\n\n";

    bmp.DrawCross();
    std::cout << separate_line;
    bmp.WriteInTerminal();
    std::cout << separate_line;

    std::cout << "Enter output BMP file name: ";
    std::getline(std::cin, output_file_path);
	bmp.WriteInFile(output_file_path);
}