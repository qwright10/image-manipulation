//
// Created by lcadmin on 20.03.24.
//

#include "blend.h"

const RGB<> blend_multiply(const RGB<>& layer1, const RGB<>& layer2) {
    auto normal1 = layer1.normalized();
    auto normal2 = layer2.normalized();

    return RGB<double>{
            normal1.r * normal2.r,
            normal1.g * normal2.g,
            normal1.b * normal2.b,
    }.denormalized();
}

const RGB<> blend_screen(const RGB<>& layer1, const RGB<>& layer2) {
    auto normal1 = layer1.normalized();
    auto normal2 = layer2.normalized();

    return RGB<double>{
            1 - ((1 - normal1.r) * (1 - normal2.r)),
            1 - ((1 - normal1.g) * (1 - normal2.g)),
            1 - ((1 - normal1.b) * (1 - normal2.b))
    }.denormalized();
}

const RGB<> blend_subtract(const RGB<>& layer1, const RGB<>& layer2) {
    auto c1 = layer1.unclamped();
    auto c2 = layer2.unclamped();

    return RGB<int>{
            c1.r - c2.r,
            c1.g - c2.g,
            c1.b - c2.b,
    }.clamped();
}

const RGB<> blend_addition(const RGB<>& layer1, const RGB<>& layer2) {
    const auto c1 = layer1.unclamped();
    const auto c2 = layer2.unclamped();

    return RGB<int>{
            c1.r + c2.r,
            c1.g + c2.g,
            c1.b + c2.b,
    }.clamped();
}

const RGB<> blend_overlay(const RGB<>& layer1, const RGB<>& layer2) {
    auto n1 = layer1.normalized();
    auto r1 = n1.r, g1 = n1.g, b1 = n1.b;
    auto n2 = layer2.normalized();
    auto r2 = n2.r, g2 = n2.g, b2 = n2.b;

    double r;
    double g;
    double b;

    if (r2 > 0.5) r = 1 - (2 * (1 - r1) * (1 - r2));
    else r = 2 * r1 * r2;

    if (g2 > 0.5) g = 1 - (2 * (1 - g1) * (1 - g2));
    else g = 2 * g1 * g2;

    if (b2 > 0.5) b = 1 - (2 * (1 - b1) * (1 - b2));
    else b = 2 * b1 * b2;

    return RGB<double>{ r, g, b }.denormalized();
}
