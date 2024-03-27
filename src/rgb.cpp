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
        static_cast<double>(r / 255.0),
        static_cast<double>(g / 255.0),
        static_cast<double>(b / 255.0)
    };
}

template<typename T>
RGB<uint8_t> RGB<T>::denormalized() const  {
    return {
        static_cast<uint8_t>(r * 255 + 0.5),
        static_cast<uint8_t>(g * 255 + 0.5),
        static_cast<uint8_t>(b * 255 + 0.5)
    };
}

template<typename T>
RGB<uint8_t> RGB<T>::clamped() const {
    return {
        static_cast<uint8_t>(clamp(r, (T)0, (T)255)),
        static_cast<uint8_t>(clamp(g, (T)0, (T)255)),
        static_cast<uint8_t>(clamp(b, (T)0, (T)255))
    };
}

template<typename T>
RGB<int16_t> RGB<T>::unclamped() const {
    return {
        static_cast<int16_t>(r),
        static_cast<int16_t>(g),
        static_cast<int16_t>(b)
    };
}

template struct RGB<double>;
template struct RGB<int>;
template struct RGB<uint8_t>;
template struct RGB<int16_t>;