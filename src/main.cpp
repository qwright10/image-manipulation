//
// Created by lcadmin on 19.03.24.
//

#include "tga.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool ends_with(const std::string& str, const char* suffix) {
    if (str.length() < strlen(suffix)) return false;
    return !str.compare(str.length() - strlen(suffix), str.length(), suffix);
}

void print_help() {
    std::cout << "Project 2: Image Processing, Spring 2024\n\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]" << std::endl;
}

int main(int argc, char* _argv[]) {
    std::vector<std::string> argv;
    for (auto i = 0; i < argc; i++) {
        argv.push_back(std::string(_argv[i]));
    }

    if (argc == 1) {
        print_help();
        return 0;
    }

    if (argc == 2) {
        if (!argv[1].compare("--help")) {
            print_help();
            return 0;
        }
    }

    const auto output = argv[1];
    if (!ends_with(output, ".tga")) {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }

    std::string inputPath;
    if (argc >= 3) {
        inputPath = argv[2];
        if (!ends_with(inputPath, ".tga")) {
            std::cerr << "Invalid file name." << std::endl;
            return 1;
        }
    }

    std::ifstream inputFile;
    inputFile.open(inputPath);
    if (!inputFile.is_open()) {
        std::cerr << "File does not exist." << std::endl;
        return 1;
    }

    auto tracking = TGA(inputFile);

    int arg_i = 3;

    auto read_string = [&arg_i, argc, argv](bool reject) mutable -> std::string* {
        if (arg_i == argc) {
            if (reject) {
                std::cerr << "Missing argument." << std::endl;
                exit(1);
            }

            return nullptr;
        }

        return &argv[arg_i++];
    };

    auto read_int = [argv, read_string]() mutable -> int {
        const auto str = read_string(true);

        try {
            return clamp(std::stoi(str->c_str()), 0, 255);
        } catch (const std::exception& error) {
            std::cerr << "Invalid argument, expected number." << std::endl;
            exit(1);
        }
    };

    auto read_file_into_tga = [&arg_i, argc, argv]() mutable -> TGA {
        if (arg_i == argc) {
            std::cerr << "Missing argument." << std::endl;
            exit(1);
        }

        const auto filename = argv[arg_i++];
        if (filename == "") {
            std::cerr << "Missing argument." << std::endl;
            exit(1);
        }

        if (!ends_with(filename, ".tga")) {
            std::cerr << "Invalid argument, invalid file name." << std::endl;
            exit(1);
        }

        std::ifstream file;
        file.open(filename);
        if (!file.is_open()) {
            std::cerr << "Invalid argument, file does not exist.";
        }

        const TGA image(file);
        file.close();

        return image;
    };

    while (true) {
        const auto method = read_string(false);

        if (method == nullptr) break;

        if (*method == "multiply") {
            const auto other = read_file_into_tga();
            tracking = tracking.multiply(other);
        } else if (*method == "subtract") {
            const auto bottom = read_file_into_tga();
            tracking = bottom.subtract(tracking);
        } else if (*method == "overlay") {
            const auto bottom = read_file_into_tga();
            tracking = bottom.overlay(tracking);
        } else if (*method == "screen") {
            const auto top = read_file_into_tga();
            tracking = tracking.screen(top);
        } else if (*method == "combine") {
            const auto green = read_file_into_tga();
            const auto blue = read_file_into_tga();
            tracking = tracking.add(green).add(blue);
        } else if (*method == "flip") {
            tracking = tracking.flipped();
        } else if (*method == "onlyred") {
            tracking = tracking.monochrome(0);
        } else if (*method == "onlygreen") {
            tracking = tracking.monochrome(1);
        } else if (*method == "onlyblue") {
            tracking = tracking.monochrome(2);
        } else if (*method == "addred") {
            const auto delta = read_int();
            tracking = tracking.adding(delta, 0, 0);
        } else if (*method == "addgreen") {
            const auto delta = read_int();
            tracking = tracking.adding(0, delta, 0);
        } else if (*method == "addblue") {
            const auto delta = read_int();
            tracking = tracking.adding(0, 0, delta);
        } else if (*method == "scalered") {
            const auto scale = read_int();
            tracking = tracking.scaling(scale, 0, 0);
        } else if (*method == "scalegreen") {
            const auto scale = read_int();
            tracking = tracking.scaling(0, scale, 0);
        } else if (*method == "scaleblue") {
            const auto scale = read_int();
            tracking = tracking.scaling(0, 0, scale);
        } else {
            std::cerr << "Invalid method name." << std::endl;
            return 1;
        }
    }

    std::cout << "↪ Writing output to " << output << std::endl;
    tracking.write(output.c_str());

    return 0;
}