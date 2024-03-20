//
// Created by lcadmin on 20.03.24.
//

#ifndef IMAGE_PROCESSING_BLEND_H
#define IMAGE_PROCESSING_BLEND_H

#include "rgb.h"

enum BlendMode { multiply, screen, subtract, addition, overlay };

const RGB<> blend_addition(const RGB<>& layer1, const RGB<>& layer2);
const RGB<> blend_multiply(const RGB<>& layer1, const RGB<>& layer2);
const RGB<> blend_overlay(const RGB<>& layer1, const RGB<>& layer2);
const RGB<> blend_screen(const RGB<>& layer1, const RGB<>& layer2);
const RGB<> blend_subtract(const RGB<>& layer1, const RGB<>& layer2);

#endif //IMAGE_PROCESSING_BLEND_H
