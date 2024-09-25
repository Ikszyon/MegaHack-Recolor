#pragma once

#include <string>

#include "animationhandler.hpp"
#include "colorfade.hpp"
#include "gui.hpp"
#include "mhcolor.hpp"
#include "mhinfo.hpp"
#include "mhmenu.hpp"
#include "savedata.hpp"

namespace MHRecolor {
    class Manager {
       private:
        static MHInfo Info;
        static MHMenu Menu;
        static GUI MHRCGUI;
        static Errorcode Errors;

        static std::vector<AnimationData> Animations;

       public:
        static Errorcode Init();
        static void ShowErrorMessage();

        static void LoadSave();

        static void SaveSave(); // 🔥 🔥 🔥

        static bool AddAnimation(AnimationData& Animation);
        static void SetColorDesired();
        static void PlayAnimation(std::string AnimationName);
        static void UpdateMenu();

       private:
        static void GUIThread();

       public:
        static void StartGUIThread();
    };
}