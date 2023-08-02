#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <mhrecolor.hpp>

#include <extensions2.hpp>

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>


using namespace MegaHackExt;


DWORD APIENTRY MainThread(LPVOID lpParam) {

    if(!MegaHackRecolor::Init()) {
        return 1;
    }

    if(!MegaHackRecolor::SignaturesFound) {
        MegaHackRecolor::SigsNotFoundWindow();
        return 1;
    }

    MegaHackRecolor::LoadConfig();
    MegaHackRecolor::SetupHooks();
    MegaHackRecolor::CreateModWindow();

    for(int i = 0 ;; i = (i + MegaHackRecolor::RainbowBool) % 361) {
        Sleep((16.0 / MegaHackRecolor::RainbowSpeed));

        if(MegaHackRecolor::RainbowBool) {
            MegaHackRecolor::MHColor = MegaHackRecolor::Util::HSVtoRGB(static_cast<float>(i), MegaHackRecolor::RainbowSaturation, MegaHackRecolor::RainbowValue);
            MegaHackRecolor::Picker->set(MegaHackRecolor::MHColor);
        }
    }

    return 0;
}

DWORD WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
    switch(dwReason) {
        case DLL_PROCESS_ATTACH:
            CreateThread(NULL, 0x1000, reinterpret_cast<LPTHREAD_START_ROUTINE>(&MainThread), NULL, 0, NULL);
            break;

        default:
            break;
    }

    return TRUE;
}