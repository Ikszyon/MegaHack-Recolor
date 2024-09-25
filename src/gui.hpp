#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <imgui-cocos.hpp>

#include "mhcolor.hpp"
#include "errorcodes.hpp"

namespace MHRecolor {
    class GUI {
       private:
        static bool IsInitialized;
        static float DesiredColor[3];
        static ImVec4* GUIColors;
        static bool RainbowEnabled;


        bool LastVisibility = 0;

       public:
        GUI() = default;

        Errorcode Init();

        bool SetVisible(bool SetVisibility);

        void SetGUIColor(MHColor Color);

        MHColor GetDesiredColor();

        void SetDesiredColor(MHColor Color);

        bool GetRainbowEnabled();

        void SetRainbowEnabled(bool Bool);
    };
}