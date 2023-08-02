#define __midhook void __declspec(naked)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#include <Psapi.h>

#include <extensions2.hpp>

#include <json.hpp>

#include <minhook.h>

#include <Sig.hpp>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace MegaHackExt;


namespace MegaHackRecolor {

    static char* Base;

    static Colour MHColor = {0xAD, 0x62, 0xEE};
    static Colour LastColor = {0xAD, 0x62, 0xEE};

    static int RainbowSpeed = 1;
    static int RainbowSaturation = 100;
    static int RainbowValue = 100;
    static bool RainbowBool = 0;

    static uintptr_t MHTitleBar_o = 0;
    static uintptr_t MHDrawGUI_o = 0;
    static bool SignaturesFound = 0;

    static ColourPicker* Picker;

    struct MidHook { // Target, ReturnOffset, Detour, DetourSize
        uintptr_t Target;
        uintptr_t ReturnOffset;

        uintptr_t Detour;
        int DetourSize;

        void PlaceHook() {
            MH_CreateHook(reinterpret_cast<void*>(Base + Target),
                          reinterpret_cast<void*>(Detour),
                          NULL);

            DWORD OldProt;
            VirtualProtect(reinterpret_cast<void*>(Detour), DetourSize, PAGE_EXECUTE_READWRITE, &OldProt);
            *(uintptr_t*)(Detour + DetourSize - 0x4) = (uintptr_t)(Base + Target + ReturnOffset);
            VirtualProtect(reinterpret_cast<void*>(Detour), DetourSize, OldProt, &OldProt);
        }
    };

    namespace Util {
        int FindInMHDLL(const char* Haystack) {
            return std::search(Base, Base + 0x800000, Haystack, Haystack + strlen(Haystack)) - Base;
        }

        Colour HSVtoRGB(float H, float S, float V) {
            if(H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0) {
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
    }

    namespace Hooks {
        __midhook Titlebar() {
            __asm {
                movzx eax, BYTE PTR [MHColor + 2]
                push eax
                movzx eax, BYTE PTR [MHColor + 1]
                push eax
                movzx eax, BYTE PTR [MHColor]
                push eax

                // I have to do this cus shitass c++ inline asm
                // doesnt let me do jmp 0x12345678
                push 0x12345678
                ret
            }
        }

        __midhook CheckBoxIndicator() {
            __asm {
                push [MHColor + 2]
                push [MHColor + 1]
                push [MHColor]

                push 0xDEADBEEF
                ret
            }
        }

        __midhook CheckBoxIndicatorOpaque() {
            __asm {
                push [MHColor + 2]
                push [MHColor + 1]
                push [MHColor]

                push 0xAFDAFD88
                ret
            }
        }

        __midhook EnabledText() {
            __asm {
                push edx

                mov edx, dword ptr [MHColor]
                or edx, 0xFF000000
                mov [ecx + eax * 8 + 0x5050], edx

                pop edx

                push 0x18718769
                ret
            }
        }

        __midhook EnabledTextOpaque() {
            __asm {
                push edx

                mov edx, dword ptr [MHColor]
                and edx, 0x00FFFFFF
                or edx, 0x40000000
                mov [ecx + eax * 8 + 0x5050], edx

                pop edx

                push 0x12341234
                ret
            }
        }

        __midhook SelectionBoxText() {
            __asm {
                push ecx

                cmp ecx, 0xFFE3EBEB
                je movval

                mov ecx, dword ptr [MHColor]
                or ecx, 0xFF000000

              movval:
                mov [edx + eax * 8 + 0x5050], ecx

                pop ecx
                push 0x18769187
                ret
            }
        }

        __midhook SelectionBoxIndicator() {
            __asm {
                push [MHColor + 2]
                push [MHColor + 1]
                push [MHColor]

                push 0x99999999
                ret
            }
        }
    }

    void SaveConfig() {
        nlohmann::json Data;

        Data["MHColor"] = (RainbowBool ? LastColor : MHColor).toHexString();

        Data["RainbowSpeed"] = RainbowSpeed;
        Data["RainbowSaturation"] = RainbowSaturation;
        Data["RainbowValue"] = RainbowValue;
        Data["RainbowBool"] = RainbowBool;

        if(!std::filesystem::is_directory("MHRecolor")) {
            std::filesystem::create_directory("MHRecolor");
        }

        std::ofstream ConfigFile("MHRecolor/config.json", std::ios::out | std::ios::trunc);
        ConfigFile << Data.dump(4);
        ConfigFile.close();
    }

    void LoadConfig() {
        if(!std::filesystem::exists("MHRecolor/config.json")) {
            SaveConfig();
            return;
        }

        std::ifstream ConfigFile("MHRecolor/config.json");

        try {
            nlohmann::json Data = nlohmann::json::parse(ConfigFile);

            MHColor.fromHexString(Data["MHColor"]);
            LastColor = MHColor;

            RainbowSpeed = Data["RainbowSpeed"];
            RainbowSaturation = Data["RainbowSaturation"];
            RainbowValue = Data["RainbowValue"];
            RainbowBool = Data["RainbowBool"];
        } catch (std::exception& E) {
            MHColor = {0xAD, 0x62, 0xEE};
            LastColor = MHColor;

            RainbowSpeed = 1.0;
            RainbowSaturation = 100;
            RainbowValue = 100;
            RainbowBool = 0;
        }

        ConfigFile.close();
    }

    bool Init() {
        if(!std::filesystem::exists("hackpro.dll")) {
            return 0;
        }

        Base = reinterpret_cast<char*>(GetModuleHandle("hackpro.dll"));

        MODULEINFO HackProInfo = {0};
        if(GetModuleInformation(GetCurrentProcess(), (HMODULE)Base, &HackProInfo, sizeof(HackProInfo))) {
            MHTitleBar_o = (uintptr_t)Sig::find(Base, HackProInfo.SizeOfImage, "55 8B EC 83 E4 F8 51 53 56 8B D9") - (uintptr_t)Base;
            MHDrawGUI_o = (uintptr_t)Sig::find(Base, HackProInfo.SizeOfImage, "55 8B EC 83 E4 F8 83 EC 0C 53 8B D9 56 57 8B 3D ? ? ? ?") - (uintptr_t)Base;

            MH_Initialize();
        } else {
            return 0;
        }

        if(MHTitleBar_o == 0 ||
           MHDrawGUI_o == 0) {
            return 0;
        } else {
            SignaturesFound = 1;
            return 1;
        }
    }

    void SetupHooks() {
        std::vector<MidHook> Hooks =
        {
            {MHTitleBar_o + 0x15, 0x18,
             reinterpret_cast<uintptr_t>(&Hooks::Titlebar), 0x1D},

            {MHDrawGUI_o + 0x9D2, 0xC,
             reinterpret_cast<uintptr_t>(&Hooks::CheckBoxIndicator), 0x17},

            {MHDrawGUI_o + 0xA05, 0xC,
             reinterpret_cast<uintptr_t>(&Hooks::CheckBoxIndicatorOpaque), 0x17},

            {MHDrawGUI_o + 0x9F3, 0xB,
             reinterpret_cast<uintptr_t>(&Hooks::EnabledText), 0x1A},

            {MHDrawGUI_o + 0xA22, 0xB,
             reinterpret_cast<uintptr_t>(&Hooks::EnabledTextOpaque), 0x20},

            {MHDrawGUI_o + 0x24E, 0x7,
             reinterpret_cast<uintptr_t>(&Hooks::SelectionBoxText), 0x22},

            {MHDrawGUI_o + 0x334, 0xC,
             reinterpret_cast<uintptr_t>(&Hooks::SelectionBoxIndicator), 0x17}
        };

        for(auto& i : Hooks) {
            i.PlaceHook();
        }

        MH_EnableHook(MH_ALL_HOOKS);
    }

    void CreateModWindow() {
        Window* Window = Window::Create("MH Recolor");

        Picker = ColourPicker::CreateEx({0xAD, 0x62, 0xEE}, MHColor,
        [](ColourPicker*, Colour Color) {
            MHColor = Color;
            if(!RainbowBool) {
                LastColor = Color;
                SaveConfig();
            }
        });

        CheckBox* RainbowCheckbox = CheckBox::CreateEx("Rainbow", RainbowBool, 1,
        [](CheckBox* Self, bool Toggle) {
            RainbowBool = Toggle;
            if(!Toggle) {
                Picker->set(LastColor);
            }
            SaveConfig();
        });

        Spinner* Saturation = Spinner::CreateEx("S: ", "%", RainbowSaturation, 1,
        [](Spinner* Self, double Value) {
            RainbowSaturation = (Value > 100) ? 100 : (Value < 0) ? 0 : Value;
            Self->set(RainbowSaturation, 0);
            SaveConfig();
        });

        Spinner* Value = Spinner::CreateEx("V: ", "%", RainbowValue, 1,
        [](Spinner* Self, double Value) {
            RainbowValue = (Value > 100) ? 100 : (Value < 0) ? 0 : Value;
            Self->set(RainbowValue, 0);
            SaveConfig();
        });

        Spinner* Speed = Spinner::CreateEx("Speed: ", "x", RainbowSpeed, 1,
        [](Spinner* Self, double Value) {
            RainbowSpeed = (Value > 16) ? 16 : (Value < 1) ? 1 : Value;
            Self->set(RainbowSpeed, 0);
            SaveConfig();
        });

        Window->addElements({Picker,
                             HorizontalLayout::Create(RainbowCheckbox, Speed),
                             HorizontalLayout::Create(Saturation, Value)});
    }

    void SigsNotFoundWindow() {
        Window* Window = Window::Create("MH Recolor");

        Label* VersionLabel1 = Label::Create("Function Sigs not found");
        Label* VersionLabel2 = Label::Create("MH Recolor likely outdated");

        Button* GitHub = Button::CreateEx("GitHub",
        [](Button*) {
            ShellExecute(NULL, NULL, "https://github.com/Ikszyon/MegaHack-Recolor/releases/latest", NULL, NULL, SW_SHOW);
        });

        Window->addElements({VersionLabel1,
                             VersionLabel2,
                             GitHub});
    }

}