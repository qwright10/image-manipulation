//
// Created by lcadmin on 20.03.24.
//

#ifndef IMAGE_PROCESSING_STREAMUTILS_H
#define IMAGE_PROCESSING_STREAMUTILS_H

#include <fstream>

template<typename T>
T read_stream(std::basic_ifstream<char>& stream);

template<typename T>
void write_stream(std::basic_ofstream<char>& stream, T value);

#endif //IMAGE_PROCESSING_STREAMUTILS_H
