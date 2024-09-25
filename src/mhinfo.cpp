#include "mhinfo.hpp"

/*
    Not needed anymore but I'll leave it here in case I need it again

    Ik this is hacky asf but I didn't feel like dealing with mutexes
    #define WAIT_UNTIL_TRUE(Condition)               \
        do {                                         \
            for(volatile bool B; B = !(Condition);); \
        } while(0)
*/

namespace MHRecolor {
    template<typename T>
    T MHInfo::GetPtrFromRIPDepMOV(uintptr_t InstructionAddress, uintptr_t InstructionSize) {
        uintptr_t RIP = InstructionAddress + InstructionSize;
        uintptr_t Offset = *reinterpret_cast<DWORD*>(InstructionAddress + InstructionSize - 0x04);

        return reinterpret_cast<T>(RIP + Offset);
    }

    bool MH__MenuIsOpen = 0;
    void MH__SetMenuVisible(uintptr_t RCX, float XMM1) {
        *(float*)(RCX + 0x18C) = XMM1;                // | movss [rcx+Geode.s_addressStack+14],xmm1
        MH__MenuIsOpen = *(bool*)(RCX + 0x18F);       // + Lowest byte of XMM1 - Actually a float but its only a non-zero bool when open as well
        return;                                       // | ret
    }

    Errorcode MHInfo::Init() {
        MHBase = reinterpret_cast<uintptr_t>(GetModuleHandle("absolllute.megahack.dll"));

        if(MHBase == NULL) {
            return Errorcode::MHNotFound;
        }

        MODULEINFO MHDLLInfo = {0};
        if(!GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(MHBase), &MHDLLInfo, sizeof(MHDLLInfo))) {
            return Errorcode::MHDLLInfoError;
        }

        MHDrawGUI_o = (uintptr_t)Sig::find(reinterpret_cast<void*>(MHBase), MHDLLInfo.SizeOfImage, "0F B6 41 23 0F 57 D2");
        MHRandomAssFunc_o = (uintptr_t)Sig::find(reinterpret_cast<void*>(MHBase), MHDLLInfo.SizeOfImage, "F3 0F 11 89 ? ? ? ? C3");

        if(!MHDrawGUI_o || !MHRandomAssFunc_o) {
            return Errorcode::MHSigsNotFound;
        }

        MHMenuColor = GetPtrFromRIPDepMOV<MHColor*>(MHDrawGUI_o + 0x0F, 0x06);
        MHMenuColorT = GetPtrFromRIPDepMOV<MHColor*>(MHDrawGUI_o + 0x4D, 0x06);
        MHMenuIsOpen = &MH__MenuIsOpen;

        if(!MHMenuColor || !MHMenuColorT || !MHMenuIsOpen) {
            return Errorcode::MHVarsNotFound;
        }

        auto Result = Mod::get()->hook(
            reinterpret_cast<void*>(MHRandomAssFunc_o),
            &MH__SetMenuVisible,
            "MH::SetMenuVisible",
            tulip::hook::TulipConvention::Fastcall);

        return Errorcode::None;
    }

    MHColor MHInfo::GetMenuColor() {
        return *MHMenuColor;
    }

    MHColor MHInfo::GetMenuColorT() {
        return *MHMenuColorT;
    }

    void MHInfo::SetMenuColor(MHColor Color) {
        *MHMenuColor = Color;
    }

    void MHInfo::SetMenuColorT(MHColor Color) {
        *MHMenuColorT = Color;
    }

    bool MHInfo::GetMenuState() {
        return *MHMenuIsOpen;
    }

    bool MHInfo::InitSuccess() {
        return Success;
    }
}