#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <extensions2.hpp>

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <fstream>


using namespace MegaHackExt;


static auto HackPro = reinterpret_cast<char*>(GetModuleHandle("hackpro.dll"));

static ColourPicker* ColorPicker = ColourPicker::Create({0xAD, 0x62, 0xEE});

static bool Deez = Client::getModEnabled("Nuts"); // Do not delete it might seem useless but its not


size_t GetOffset(const char* Buffer, size_t Length, const char* String) {
    return std::search(Buffer, Buffer + Length, String, String + strlen(String)) - Buffer;
}

void Patch(void* loc, std::vector<uint8_t> Bytes) {
    auto size = Bytes.size();
    DWORD old_prot;
    VirtualProtect(loc, size, PAGE_EXECUTE_READWRITE, &old_prot);
    memcpy(loc, Bytes.data(), size);
    VirtualProtect(loc, size, old_prot, &old_prot);
}

std::vector<uint8_t> StringToVec(std::string String) {
    std::vector<uint8_t> Temp;

    for(int i = 0; i < String.length(); i += 2) {
        unsigned int Char;
        std::istringstream Hex(String.substr(i, 2));
        Hex >> std::hex >> Char;
        Temp.push_back(Char);
    }

    return Temp;
}

void WriteConfig(std::string Content) {
    std::ofstream WriteColorFile("MH_COLOR", std::ios::out | std::ios::trunc);
    WriteColorFile << Content;
    WriteColorFile.close();
}

void MH_CALL SetColor(Button* Btn) {
    WriteConfig(ColorPicker->getHexString());
}

bool LoadedWithQuickLDR() {
    std::ifstream SettingsFile("quickldr/settings.txt", std::ios::in);
    std::stringstream SStr;
    SStr << SettingsFile.rdbuf();
    std::string SettingsContent = SStr.str();
    SettingsFile.close();

    return (SettingsContent.find("MegaHack-Recolor.dll") != std::string::npos);
}

DWORD WINAPI MainThread(LPVOID lpParam) {
    std::ifstream ColorFile("MH_COLOR", std::ios::in);
    std::stringstream SStr;
    SStr << ColorFile.rdbuf();
    std::string Hex = SStr.str();
    ColorFile.close();

    if(Hex.length() != 6) {
        Hex = "AD62EE";
    }

    ColorPicker->setHexString(Hex);
    std::vector<uint8_t> Color = StringToVec(Hex);

    for(int i = 0; i < 3; i++) {
            Patch(HackPro + GetOffset(HackPro, 0x800000, "\xAD\x62\xEE"), Color);
            Patch(HackPro + GetOffset(HackPro, 0x800000, "\x6A\x62\x68\xAD") - 0x04, {Color[2], 0x00, 0x00, 0x00, 0x6a, Color[1], 0x68, Color[0]});
    }

    Window* MHRecolor = Window::Create("MH Recolor");
    Label* InfoLabel = Label::Create("Change requires Restart");
    Button* Set = Button::WithCallback("Set Color", SetColor);

    Label* QuickLDR = Label::Create("Load me with quickldr");

    if(LoadedWithQuickLDR()) {
        MHRecolor->addElements({InfoLabel,
                                ColorPicker,
                                Set});
    } else {
        MHRecolor->addElements({QuickLDR});
    }

    return S_OK;
}

DWORD APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
    switch(dwReason) {
        case DLL_PROCESS_ATTACH:
            CreateThread(NULL, 0x1000, reinterpret_cast<LPTHREAD_START_ROUTINE>(&MainThread), NULL, 0, NULL);
            break;

        default:
            break;
    }

    return TRUE;
}