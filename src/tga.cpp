//
// Created by lcadmin on 19.03.24.
//

#include <fstream>
#include <iostream>
#include <vector>

#include "rgb.h"
#include "streamutils.h"
#include "tga.h"

TGA::Header::Header(const TGA::Header &copying) {
    idLength = copying.idLength;
    colorMapType = copying.colorMapType;
    imageType = copying.imageType;
    colorMapStart = copying.colorMapStart;
    colorMapLength = copying.colorMapLength;
    colorMapBitDepth = copying.colorMapBitDepth;
    xOrigin = copying.xOrigin;
    yOrigin = copying.yOrigin;
    width = copying.width;
    height = copying.height;
    bitDepth = copying.bitDepth;
    descriptor = copying.descriptor;
}

TGA::Header::Header(std::basic_ifstream<char> &stream) {
    auto read_8 = [&stream]() { return read_stream_u8(stream); };
    auto read_16 = [&stream]() { return read_stream_u16(stream); };

    idLength = read_8();
    colorMapType = read_8();
    imageType = read_8();
    colorMapStart = read_16();
    colorMapLength = read_16();
    colorMapBitDepth = read_8();
    xOrigin = read_16();
    yOrigin = read_16();
    width = read_16();
    height = read_16();
    bitDepth = read_8();
    descriptor = read_8();
}

int TGA::Header::size() const {
    return width * height;
}

TGA TGA::from_file(const char *path) {
    std::ifstream file;
    file.open(path);

    if (!file || !file.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    TGA image(file);
    file.close();
    return image;
}

TGA TGA::blend(const BlendMode mode, const TGA &top, const TGA &bottom) {
    Header header(top.header);
    const auto size = header.width * header.height;
    std::vector<RGB<>> data;

    auto blend_method = blend_addition;

    if (mode == BlendMode::addition) blend_method = blend_addition;
    if (mode == BlendMode::multiply) blend_method = blend_multiply;
    if (mode == BlendMode::overlay) blend_method = blend_overlay;
    if (mode == BlendMode::screen) blend_method = blend_screen;
    if (mode == BlendMode::subtract) blend_method = blend_subtract;

    for (auto i = 0; i < size; i++) {
        data.push_back(blend_method(top.data[i], bottom.data[i]));
    }

    return TGA(Header(header), data);
}

TGA TGA::adding(int16_t dR, int16_t dG, int16_t dB) const {
    const auto size = header.size();
    std::vector<RGB<>> nd;

    for (auto i = 0; i < size; i++) {
        auto pixel = data[i].unclamped();
        nd[i] = RGB<int16_t>{
                (short)(pixel.r + dR),
                (short)(pixel.g + dG),
                (short)(pixel.b + dB),
        }.clamped();
    }

    return TGA(Header(header), nd);
}

TGA TGA::extract_channel(const int channel) const {
    const auto size = header.size();
    std::vector<RGB<>> nd;

    for (auto i = 0; i < size; i++) {
        const auto pixel = data[i];
        nd[i] = RGB<>{
                static_cast<uint8_t>(channel == 0 ? pixel.r : 0),
                static_cast<uint8_t>(channel == 1 ? pixel.g : 0),
                static_cast<uint8_t>(channel == 2 ? pixel.b : 0),
        };
    }

    return TGA(Header(header), nd);
}

TGA TGA::flipped() const {
    std::vector<RGB<>> nd;

    //for (auto i = 0; i < SIZE; i++) {
    //    nd[i] = data[SIZE - i - 1];
    //}

    return TGA(Header(header), nd);
}

TGA TGA::monochrome(const int channel) const {
    const auto size = header.size();
    std::vector<RGB<>> nd;

    for (auto i = 0; i < size; i++) {
        const auto pixel = data[i];
        uint8_t value = 0;
        if (channel == 0) value = pixel.r;
        if (channel == 1) value = pixel.g;
        if (channel == 2) value = pixel.b;

        nd[i] = RGB<>{ value, value, value };
    }

    return TGA(Header(header), nd);
}

TGA TGA::scaling(int16_t sR, int16_t sG, int16_t sB) const {
    const auto size = header.size();
    std::vector<RGB<>> nd;

    for (auto i = 0; i < size; i++) {
        auto pixel = data[i].unclamped();
        nd[i] = RGB<int16_t>{
                (short)(pixel.r * sR),
                (short)(pixel.g * sG),
                (short)(pixel.b * sB),
        }.clamped();
    }

    return TGA(Header(header), nd);
}

TGA TGA::add(const TGA &top) const {
    return TGA::blend(BlendMode::addition, top, *this);
}

TGA TGA::multiply(const TGA &top) const {
    return TGA::blend(BlendMode::multiply, top, *this);
}

TGA TGA::overlay(const TGA &top) const {
    return TGA::blend(BlendMode::overlay, top, *this);
}

TGA TGA::screen(const TGA &top) const {
    return TGA::blend(BlendMode::screen, top, *this);
}

TGA TGA::subtract(const TGA &top) const {
    return TGA::blend(BlendMode::subtract, top, *this);
}

TGA::TGA(std::basic_ifstream<char> &stream): header(stream) {
    unsigned int dataLen = header.width * header.height;

    for (auto i = 0; i < dataLen; i++) {
        auto blue = read_stream_u8(stream);
        auto green = read_stream_u8(stream);
        auto red = read_stream_u8(stream);

        RGB<> pixel{red, green, blue};
        data.push_back(pixel);
    }
}

TGA::TGA(const TGA::Header header, std::vector<RGB<>> copying): header(header) {
    for (const auto pixel: copying) {
        data.push_back(pixel);
    }
}

void TGA::write(std::basic_ofstream<char> &stream) const {
    write_stream_u8(stream, header.idLength);
    write_stream_u8(stream, header.colorMapType);
    write_stream_u8(stream, header.imageType);
    write_stream_u16(stream, header.colorMapStart);
    write_stream_u16(stream, header.colorMapLength);
    write_stream_u8(stream, header.colorMapBitDepth);
    write_stream_u16(stream, header.xOrigin);
    write_stream_u16(stream, header.yOrigin);
    write_stream_u16(stream, header.width);
    write_stream_u16(stream, header.height);
    write_stream_u8(stream, header.bitDepth);
    write_stream_u8(stream, header.descriptor);

    const auto s = header.width * header.height;
    for (auto i = 0; i < s; i++) {
        const auto pixel = data[i];
        const uint8_t red = pixel.r;
        const uint8_t green = pixel.g;
        const uint8_t blue = pixel.b;

        write_stream_u8(stream, blue);
        write_stream_u8(stream, green);
        write_stream_u8(stream, red);
    }
}

void TGA::write(const char *s) const {
    std::ofstream file(s);

    write(file);
    file.close();
}
