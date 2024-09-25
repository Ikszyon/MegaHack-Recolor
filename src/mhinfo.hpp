#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <Sig.hpp>

#include "mhcolor.hpp"
#include "errorcodes.hpp"

#include <Geode/Geode.hpp>
using namespace geode::prelude;

namespace MHRecolor {
    class MHInfo {
       public:
        uintptr_t MHBase = 0;

        uintptr_t MHDrawGUI_o = 0;
        uintptr_t MHRandomAssFunc_o = 0;

        MHColor* MHMenuColor = 0;
        MHColor* MHMenuColorT = 0;
        bool* MHMenuIsOpen = 0;

        bool Success = 0;

       private:
        // Gets Final Pointer from an RIP dependant MOV instruction as type T
        template<typename T>
        T GetPtrFromRIPDepMOV(uintptr_t InstructionAddress, uintptr_t InstructionSize);

       public:
        MHInfo() = default;

        Errorcode Init();

        MHColor GetMenuColor();

        MHColor GetMenuColorT();

        void SetMenuColor(MHColor Color);

        void SetMenuColorT(MHColor Color);

        bool GetMenuState();

        bool InitSuccess();
    };
}