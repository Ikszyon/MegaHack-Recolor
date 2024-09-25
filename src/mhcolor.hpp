#pragma once

#include <initializer_list>
#include <cmath>

namespace MHRecolor {
    enum ColorType : int {
        RGB = 1 << 0,
        HSV = 1 << 1
    };

    struct MHColorHSV {
        float H, S ,V;

        MHColorHSV(float H = 0.f, float S = 0.f, float V = 0.f);
        MHColorHSV(std::initializer_list<float> InitList);
    };


    struct MHColor {
        unsigned char r, g, b;

        MHColor& operator*=(MHColor& OtherColor);

        MHColor(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);
        MHColor(std::initializer_list<unsigned char> InitList);
    };

    MHColor operator*(MHColor MainColor, MHColor& OtherColor);

    MHColor HSVToRGB(MHColorHSV Color);

    MHColorHSV RGBToHSV(MHColor Color);
}