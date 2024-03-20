//
// Created by lcadmin on 19.03.24.
//

#include <fstream>
#include <iostream>

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
    auto read_8 = [&stream]() { return read_stream<uint8_t>(stream); };
    auto read_16 = [&stream]() { return read_stream<uint16_t>(stream); };

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
    std::cout << "Reading: " << path << std::endl;

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
    RGB<>* data = new RGB<>[size];

    auto blend_method = blend_addition;

    if (mode == BlendMode::addition) blend_method = blend_addition;
    if (mode == BlendMode::multiply) blend_method = blend_multiply;
    if (mode == BlendMode::overlay) blend_method = blend_overlay;
    if (mode == BlendMode::screen) blend_method = blend_screen;
    if (mode == BlendMode::subtract) blend_method = blend_subtract;

    for (auto i = 0; i < size; i++) {
        data[i] = blend_method(top.data[i], bottom.data[i]);
    }

    return TGA{ header, data };
}

TGA TGA::adding(int16_t dR, int16_t dG, int16_t dB) const {
    const auto size = header.size();
    RGB<>* nd = new RGB<>[size];

    for (auto i = 0; i < size; i++) {
        auto pixel = data[i].unclamped();
        nd[i] = RGB<int16_t>{
                (short)(pixel.r + dR),
                (short)(pixel.g + dG),
                (short)(pixel.b + dB),
        }.clamped();
    }

    return TGA{Header(header), nd};
}

TGA TGA::extract_channel(const int channel) const {
    const auto size = header.size();
    RGB<>* nd = new RGB<>[size];

    for (auto i = 0; i < size; i++) {
        const auto pixel = data[i];
        nd[i] = RGB<>{
                (uint8_t)(channel == 0 ? pixel.r : 0),
                (uint8_t)(channel == 1 ? pixel.g : 0),
                (uint8_t)(channel == 2 ? pixel.b : 0),
        };
    }

    return TGA{header, nd};
}

TGA TGA::flipped() const {
    const auto size = header.size();
    RGB<>* nd = new RGB<>[size];

    for (auto i = 0; i < size; i++) {
        nd[i] = data[size - i - 1];
    }

    return TGA{ header, nd };
}

TGA TGA::monochrome(const int channel) const {
    const auto size = header.size();
    RGB<>* nd = new RGB<>[size];

    for (auto i = 0; i < size; i++) {
        const auto pixel = data[i];
        uint8_t value = 0;
        if (channel == 0) value = pixel.r;
        if (channel == 1) value = pixel.g;
        if (channel == 2) value = pixel.b;

        nd[i] = RGB<>{ value, value, value };
    }

    return TGA{header, nd};
}

TGA TGA::scaling(int16_t sR, int16_t sG, int16_t sB) const {
    const auto size = header.size();
    RGB<>* nd = new RGB<>[size];

    for (auto i = 0; i < size; i++) {
        auto pixel = data[i].unclamped();
        nd[i] = RGB<int16_t>{
                (short)(pixel.r * sR),
                (short)(pixel.g * sG),
                (short)(pixel.b * sB),
        }.clamped();
    }

    return TGA{Header(header), nd};
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
    data = new RGB<>[dataLen];

    for (auto i = 0; i < dataLen; i++) {
        auto blue = read_stream<uint8_t>(stream);
        auto green = read_stream<uint8_t>(stream);
        auto red = read_stream<uint8_t>(stream);

        data[i] = { red, green, blue };
    }

    std::cout << "Read " << dataLen * 3 << " bytes of color data\n";
}

TGA::TGA(const TGA::Header header, const RGB<> *copying): header(header) {
    const auto size = header.size();
    data = new RGB<>[size];

    for (auto i = 0; i < size; i++) {
        data[i] = copying[i];
    }
}

TGA::~TGA() {
    delete[] data;
}

void TGA::write(std::basic_ofstream<char> &stream) const {
    write_stream<uint8_t>(stream, header.idLength);
    write_stream<uint8_t>(stream, header.colorMapType);
    write_stream<uint8_t>(stream, header.imageType);
    write_stream<uint16_t>(stream, header.colorMapStart);
    write_stream<uint16_t>(stream, header.colorMapLength);
    write_stream<uint8_t>(stream, header.colorMapBitDepth);
    write_stream<uint16_t>(stream, header.xOrigin);
    write_stream<uint16_t>(stream, header.yOrigin);
    write_stream<uint16_t>(stream, header.width);
    write_stream<uint16_t>(stream, header.height);
    write_stream<uint8_t>(stream, header.bitDepth);
    write_stream<uint8_t>(stream, header.descriptor);

    const auto s = header.width * header.height;
    for (auto i = 0; i < s; i++) {
        const auto pixel = data[i];
        const uint8_t red = pixel.r;
        const uint8_t green = pixel.g;
        const uint8_t blue = pixel.b;

        write_stream(stream, blue);
        write_stream(stream, green);
        write_stream(stream, red);
    }
}

void TGA::write(const char *s) const {
    std::ofstream file;
    file.open(s);
    if (!file || !file.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    write(file);
    file.close();
}