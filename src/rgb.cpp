//
// Created by lcadmin on 20.03.24.
//

#include "rgb.h"

template<typename T>
const T& clamp(const T& value, const T& min, const T& max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

template<typename T>
RGB<double> RGB<T>::normalized() const {
    return {
        (double)r / 255.0,
        (double)g / 255.0,
        (double)b / 255.0
    };
}

template<typename T>
RGB<uint8_t> RGB<T>::denormalized() const  {
    return {
        (uint8_t)(r * 255 + 0.5),
        (uint8_t)(g * 255 + 0.5),
        (uint8_t)(b * 255 + 0.5)
    };
}

template<typename T>
RGB<uint8_t> RGB<T>::clamped() const {
    return {
        (uint8_t)clamp(r, (T)0, (T)255),
        (uint8_t)clamp(g, (T)0, (T)255),
        (uint8_t)clamp(b, (T)0, (T)255)
    };
}

template<typename T>
RGB<int16_t> RGB<T>::unclamped() const {
    return {
        (int16_t)r,
        (int16_t)g,
        (int16_t)b
    };
}

template struct RGB<double>;
template struct RGB<int>;
template struct RGB<uint8_t>;
template struct RGB<int16_t>;