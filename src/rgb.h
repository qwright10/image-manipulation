//
// Created by lcadmin on 20.03.24.
//

#ifndef IMAGE_PROCESSING_RGB_H
#define IMAGE_PROCESSING_RGB_H

#include <cstdint>

template<typename T>
const T& clamp(const T& value, const T& min, const T& max);

template<typename T = uint8_t>
struct RGB {
    T r;
    T g;
    T b;

    RGB<double> normalized() const;
    RGB<uint8_t> denormalized() const;
    RGB<uint8_t> clamped() const;
    RGB<int16_t> unclamped() const;
};

#endif //IMAGE_PROCESSING_RGB_H
