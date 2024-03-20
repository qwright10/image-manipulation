//
// Created by lcadmin on 19.03.24.
//

#include "tga.h"

int main() {
    TGA car = TGA::from_file("./input/car.tga");
    TGA circles = TGA::from_file("./input/circles.tga");
    TGA layer1 = TGA::from_file("./input/layer1.tga");
    TGA layer2 = TGA::from_file("./input/layer2.tga");
    TGA layerBlue = TGA::from_file("./input/layer_blue.tga").extract_channel(2);
    TGA layerGreen = TGA::from_file("./input/layer_green.tga").extract_channel(1);
    TGA layerRed = TGA::from_file("./input/layer_red.tga").extract_channel(0);
    TGA pattern1 = TGA::from_file("./input/pattern1.tga");
    TGA pattern2 = TGA::from_file("./input/pattern2.tga");
    TGA text = TGA::from_file("./input/text.tga");
    TGA text2 = TGA::from_file("./input/text2.tga");

    pattern1.multiply(layer1).write("./output/part1.tga");

    layer2.subtract(car).write("./output/part2.tga");

    text.screen(layer1.multiply(pattern2)).write("./output/part3.tga");

    pattern2.subtract(layer2.multiply(circles)).write("./output/part4.tga");

    pattern1.overlay(layer1).write("./output/part5.tga");

    car.adding(0, 200, 0).write("./output/part6.tga");

    car.scaling(4, 1, 0).write("./output/part7.tga");

    car.monochrome(0).write("./output/part8_r.tga");
    car.monochrome(1).write("./output/part8_g.tga");
    car.monochrome(2).write("./output/part8_b.tga");

    layerRed.add(layerGreen).add(layerBlue).write("./output/part9.tga");

    text2.flipped().write("./output/part10.tga");
}