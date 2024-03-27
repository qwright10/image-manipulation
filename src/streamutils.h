//
// Created by lcadmin on 20.03.24.
//

#ifndef IMAGE_PROCESSING_STREAMUTILS_H
#define IMAGE_PROCESSING_STREAMUTILS_H

#include <fstream>

uint8_t read_stream_u8(std::basic_ifstream<char>& stream);
uint16_t read_stream_u16(std::basic_ifstream<char>& stream);

void write_stream_u8(std::basic_ofstream<char>& stream, uint8_t value);
void write_stream_u16(std::basic_ofstream<char>& stream, uint16_t value);

#endif //IMAGE_PROCESSING_STREAMUTILS_H
