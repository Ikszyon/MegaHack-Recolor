#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <extensions2.hpp>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace MegaHackExt;


namespace MegaHackRecolor {

    static char* Base;
    static MegaHackExt::Colour MHColor = {0xAD, 0x62, 0xEE};
    static MegaHackExt::Colour LastColor = {0xAD, 0x62, 0xEE};
    static double RainbowSpeed = 1;
    static int RainbowSaturation = 100;
    static int RainbowValue = 100;
    static bool RainbowBool;
    static ColourPicker* Picker;

    namespace {
        int FindInMHDLL(const char* Haystack) {
            return std::search(Base, Base + 0x800000, Haystack, Haystack + strlen(Haystack)) - Base;
        }
    }

    Colour HSVtoRGB(float H, float S,float V) {
        if(H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0){
            return {0, 0, 0};
        }

        float s = S/100;
        float v = V/100;
        float C = s*v;
        float X = C*(1-abs(fmod(H/60.0, 2)-1));
        float m = v-C;
        float r,g,b;
        if(H >= 0 && H < 60){
            r = C,g = X,b = 0;
        }
        else if(H >= 60 && H < 120){
            r = X,g = C,b = 0;
        }
        else if(H >= 120 && H < 180){
            r = 0,g = C,b = X;
        }
        else if(H >= 180 && H < 240){
            r = 0,g = X,b = C;
        }
        else if(H >= 240 && H < 300){
            r = X,g = 0,b = C;
        }
        else{
            r = C,g = 0,b = X;
        }
        unsigned char R = (r+m)*255;
        unsigned char G = (g+m)*255;
        unsigned char B = (b+m)*255;
        return {R, G, B};
    }

    void Patch(void* loc, std::vector<uint8_t> Bytes) {
        auto size = Bytes.size();
        DWORD old_prot;
        VirtualProtect(loc, size, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(loc, Bytes.data(), size);
        VirtualProtect(loc, size, old_prot, &old_prot);
    }

    void SetMenuColor(Colour MenuColor) {
        MHColor = MenuColor;
        Patch(Base + 0x134663, {MenuColor.b, 0x00, 0x00, 0x00, 0x6a, MenuColor.g, 0x68, MenuColor.r});
        Patch(Base + 0x13468A, {MenuColor.r, MenuColor.g, MenuColor.b});
    }

    bool Init() {
        if(!std::filesystem::exists("hackpro.dll")) {
            return 0;
        }

        while(GetModuleHandle("hackpro.dll") == 0) {};
        Base = reinterpret_cast<char*>(GetModuleHandle("hackpro.dll"));

        return 1;
    }

    // CLEAN THIS UP WTF
    void SetupPatches() {
        char OPCodes[] = {0x0F, 0xB6, 0x05, NULL, NULL, NULL, NULL, 0x50,   // movzx eax, BYTE PTR [Address of MHColor.b]; push eax
                          0x0F, 0xB6, 0x05, NULL, NULL, NULL, NULL, 0x50,   // movzx eax, BYTE PTR [Address of MHColor.g]; push eax
                          0x0F, 0xB6, 0x05, NULL, NULL, NULL, NULL, 0x50};  // movzx eax, BYTE PTR [Address of MHColor.r]; push eax

        *(uintptr_t*)((uintptr_t)OPCodes + 3) = (uintptr_t)&MHColor.b;
        *(uintptr_t*)((uintptr_t)OPCodes + 11) = (uintptr_t)&MHColor.g;
        *(uintptr_t*)((uintptr_t)OPCodes + 19) = (uintptr_t)&MHColor.r;

        uintptr_t Offset1 = FindInMHDLL("\x0F\xB6\x83\xDD");

        DWORD OldProt;
        VirtualProtect(reinterpret_cast<void*>(Base + Offset1), 24, PAGE_EXECUTE_READWRITE, &OldProt);
        memcpy(Base + Offset1, OPCodes, 24);
        VirtualProtect(reinterpret_cast<void*>(Base + Offset1), 24, OldProt, &OldProt);

    }

    void CreateModWindow() {
        Window* Window = Window::Create("MH Recolor");

        Picker = ColourPicker::WithCallback(MHColor,
        [](ColourPicker*, Colour Color) {
            SetMenuColor(Color);
            if(!RainbowBool) {
                LastColor = Color;
            }
        });

        CheckBox* RainbowCheckbox = CheckBox::WithCallback("Rainbow",
        [](CheckBox* Self, bool Toggle) {
            RainbowBool = Toggle;
            if(!Toggle) {
                Picker->set(LastColor);
            }
        });

        Spinner* Saturation = Spinner::WithCallback("S: ", "%",
        [](Spinner* Self, double Value) {
            RainbowSaturation = (Value > 100) ? 100 : (Value < 0) ? 0 : Value;
            Self->set(RainbowSaturation, 0);
        });

        Spinner* Value = Spinner::WithCallback("V: ", "%",
        [](Spinner* Self, double Value) {
            RainbowValue = (Value > 100) ? 100 : (Value < 0) ? 0 : Value;
            Self->set(RainbowValue, 0);
        });

        Spinner* Speed = Spinner::WithCallback("Speed: ", "x",
        [](Spinner* Self, double Value) {
            RainbowSpeed = (Value > 16) ? 16 : (Value < 1) ? 1 : Value;
            Self->set(RainbowSpeed, 0);
        });

        Window->addElements({Picker,
                             HorizontalLayout::Create(RainbowCheckbox, Speed),
                             HorizontalLayout::Create(Saturation, Value)});
    }

    std::string GetMHVersion() {
        return reinterpret_cast<char*>(Base + FindInMHDLL("Successfully updated to version ") + 0x20);
    }

}