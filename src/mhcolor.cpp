#include "mhcolor.hpp"

namespace MHRecolor {
    MHColor& MHColor::operator*=(MHColor& OtherColor) {
        float OriginalColor[3] = {r / 255.f, g / 255.f, b / 255.f};
        float OverlayColor[3] = {OtherColor.r / 255.f, OtherColor.g / 255.f, OtherColor.b / 255.f};

        r = static_cast<unsigned char>(255.f * (OriginalColor[0] * OverlayColor[0]));
        g = static_cast<unsigned char>(255.f * (OriginalColor[1] * OverlayColor[1]));
        b = static_cast<unsigned char>(255.f * (OriginalColor[2] * OverlayColor[2]));

        return *this;
    }

    MHColor operator*(MHColor MainColor, MHColor& OtherColor) {
        return MainColor *= OtherColor;
    }

    MHColorHSV::MHColorHSV(float H, float S, float V)
        : H(H), S(S), V(V) {
    }

    MHColor::MHColor(unsigned char r, unsigned char g, unsigned char b)
        : r(r), g(g), b(b) {
    }

    MHColorHSV::MHColorHSV(std::initializer_list<float> InitList) {
        if(InitList.size() != 3) {
            *this = MHColorHSV();
        } else {
            *this = MHColorHSV(InitList.begin()[0], InitList.begin()[1], InitList.begin()[2]);
        }
    }

    MHColor::MHColor(std::initializer_list<unsigned char> InitList) {
        if(InitList.size() != 3) {
            *this = MHColor();
        } else {
            // fuck you microsoft how difficult is it to add one operator
            *this = MHColor(InitList.begin()[0], InitList.begin()[1], InitList.begin()[2]);
        }
    }

    MHColor HSVToRGB(MHColorHSV Color) {
        float H = Color.H;
        float S = Color.S;
        float V = Color.V;

        if((H > 360) || (H < 0) || (S > 100) || (S < 0) || (V > 100) || (V < 0)) {
            return {0, 0, 0};
        }

        float s = S / 100;
        float v = V / 100;
        float C = s * v;
        float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
        float m = v - C;
        float _r, _g, _b;

        if(H >= 0 && H < 60) {
            _r = C, _g = X, _b = 0;
        } else if(H >= 60 && H < 120) {
            _r = X, _g = C, _b = 0;
        } else if(H >= 120 && H < 180) {
            _r = 0, _g = C, _b = X;
        } else if(H >= 180 && H < 240) {
            _r = 0, _g = X, _b = C;
        } else if(H >= 240 && H < 300) {
            _r = X, _g = 0, _b = C;
        } else {
            _r = C, _g = 0, _b = X;
        }
        unsigned char R = (_r + m) * 255;
        unsigned char G = (_g + m) * 255;
        unsigned char B = (_b + m) * 255;

        return {R, G, B};
    }

    MHColorHSV RGBToHSV(MHColor In) {
        MHColorHSV Out;
        float Min, Max, Delta;

        Min = In.r < In.g ? In.r : In.g;
        Min = Min < In.b ? Min : In.b;

        Max = In.r > In.g ? In.r : In.g;
        Max = Max > In.b ? Max : In.b;

        Out.V = (Max / 255.f) * 100.f;
        Delta = Max - Min;
        if(Delta < 0.00001f) {
            Out.S = 0;
            Out.H = 0;
            return Out;
        }
        if(Max > 0.f) {
            Out.S = (Delta / Max) * 100;
        } else {
            Out.S = 0.0;
            Out.H = NAN;
            return Out;
        }
        if(In.r >= Max)
            Out.H = (In.g - In.b) / Delta;
        else if(In.g >= Max)
            Out.H = 2.f + (In.b - In.r) / Delta;
        else
            Out.H = 4.f + (In.r - In.g) / Delta;

        Out.H *= 60.f;

        if(Out.H < 0.f)
            Out.H += 360.f;

        return Out;
    }
}