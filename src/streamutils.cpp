//
// Created by lcadmin on 20.03.24.
//

#include <cstring>
#include <fstream>
#include "streamutils.h"

uint8_t read_stream_u8(std::basic_ifstream<char>& stream) {
    const auto size = sizeof(uint8_t);

    char raw[size];
    stream.read(raw, size);

    uint8_t data;
    std::memcpy(&data, raw, size);

    return data;
}

uint16_t read_stream_u16(std::basic_ifstream<char>& stream) {
    const auto size = sizeof(uint16_t);

    char raw[size];
    stream.read(raw, size);

    uint16_t data;
    std::memcpy(&data, raw, size);

    return data;
}

void write_stream_u8(std::basic_ofstream<char>& stream, uint8_t value) {
    const auto s = sizeof(uint8_t);
    char src[s];

    std::memcpy(src, &value, s);
    stream.write(src, s);
}

void write_stream_u16(std::basic_ofstream<char>& stream, uint16_t value) {
    const auto s = sizeof(uint16_t);
    char src[s];

    std::memcpy(src, &value, s);
    stream.write(src, s);
}
