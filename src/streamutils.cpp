//
// Created by lcadmin on 20.03.24.
//

#include <fstream>
#include "streamutils.h"

template<typename T>
T read_stream(std::basic_ifstream<char>& stream) {
    const auto size = sizeof(T);

    char raw[size];
    stream.read(raw, size);

    T data;
    memcpy(&data, raw, size);

    return data;
}

template uint8_t read_stream(std::basic_ifstream<char>& stream);
template uint16_t read_stream(std::basic_ifstream<char>& stream);

template<typename T>
void write_stream(std::basic_ofstream<char>& stream, T value) {
    const auto s = sizeof(T);
    char src[s];

    std::memcpy(src, &value, s);
    stream.write(src, s);
}

template void write_stream(std::basic_ofstream<char>& stream, uint8_t value);
template void write_stream(std::basic_ofstream<char>& stream, uint16_t value);
