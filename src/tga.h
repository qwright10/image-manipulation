//
// Created by lcadmin on 20.03.24.
//

#ifndef IMAGE_PROCESSING_TGA_H
#define IMAGE_PROCESSING_TGA_H

#include <cstdint>
#include <fstream>
#include "blend.h"
#include "rgb.h"

struct TGA {
public:
    struct Header {
    public:
        uint8_t idLength;
        uint8_t colorMapType;
        uint8_t imageType;
        uint16_t colorMapStart;
        uint16_t colorMapLength;
        uint8_t colorMapBitDepth;
        uint16_t xOrigin;
        uint16_t yOrigin;
        uint16_t width;
        uint16_t height;
        uint8_t bitDepth;
        uint8_t descriptor;

        Header(const Header& copying);
        Header(std::basic_ifstream<char>& stream);

        int size() const;
    };

    Header header;
    RGB<>* data;

    TGA(const Header header, const RGB<>* copying);
    TGA(std::basic_ifstream<char>& stream);
    ~TGA();

    static TGA from_file(const char* path);
    static TGA blend(const BlendMode mode, const TGA& top, const TGA& bottom);

    TGA adding(int16_t dR, int16_t dG, int16_t dB) const;
    TGA extract_channel(const int channel) const;
    TGA flipped() const;
    TGA monochrome(const int channel) const;
    TGA scaling(int16_t sR, int16_t sG, int16_t sB) const;

    TGA add(const TGA& top) const;
    TGA multiply(const TGA& top) const;
    TGA overlay(const TGA& top) const;
    TGA screen(const TGA& top) const;
    TGA subtract(const TGA& top) const;

    void write(std::basic_ofstream<char>& stream) const;
    void write(const char* s) const;
};

#endif //IMAGE_PROCESSING_TGA_H
