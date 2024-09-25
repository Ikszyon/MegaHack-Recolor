#include "manager.hpp"

namespace MHRecolor{
    MHInfo Manager::Info;
    MHMenu Manager::Menu;
    GUI Manager::MHRCGUI;
    Errorcode Manager::Errors;

    std::vector<AnimationData> Manager::Animations;

    Errorcode Manager::Init() {
        Errors |= Info.Init();
        if(Errors != Errorcode::None) {
            ShowErrorMessage();
            return Errors;
        }
        Errors |= Menu.Init(Info);
        if(Errors != Errorcode::None) {
            ShowErrorMessage();
            return Errors;
        }
        Errors |= MHRCGUI.Init();
        if(Errors != Errorcode::None) {
            ShowErrorMessage();
            return Errors;
        }

        return Errorcode::None;
    }

    void Manager::ShowErrorMessage() {
        std::string ErrorMessage = "The following errors have occured:\n";

        if(Errors & Errorcode::MHNotFound) {
            ErrorMessage += "- MH couldn't be found (Check if MH is installed)\n";
        }
        if(Errors & Errorcode::MHDLLInfoError) {
            ErrorMessage += "- Couldn't retrieve MH's DLL info\n";
        }
        if(Errors & Errorcode::MHSigsNotFound) {
            ErrorMessage += "- Couldn't find all function signatures (Wait for update)\n";
        }
        if(Errors & Errorcode::MHVarsNotFound) {
            ErrorMessage += "- Couldn't find all important variables (Wait for Update) \n";
        }
        if(Errors & Errorcode::MHInfoIsNullptr) {
            ErrorMessage += "- Something went really wrong (DM me)\n";
        }

        auto Alert = geode::createQuickPopup(
            "MH Recolor",
            ErrorMessage,
            "OK", nullptr,
            [&](auto This, bool) {
                SceneManager::get()->forget(This);
            }
        );
        SceneManager::get()->keepAcrossScenes(Alert);
    }

    void Manager::LoadSave() {
        if(Errors == Errorcode::None) {
            SaveData Data = Mod::get()->getSavedValue<SaveData>("Data");

            MHRCGUI.SetDesiredColor(Data.MenuColor);
            MHRCGUI.SetRainbowEnabled(Data.RainbowEnabled);
        }
    }

    void Manager::SaveSave() {
        if(Errors == Errorcode::None) {
            SaveData Data(MHRCGUI.GetDesiredColor(), MHRCGUI.GetRainbowEnabled());

            Mod::get()->setSavedValue("Data", Data);
        }
    }

    bool Manager::AddAnimation(AnimationData& Animation) {
        for(auto& CurrentAnimation : Animations) {
            if(CurrentAnimation.GetName() == Animation.GetName()) {
                return 0;
            }
        }
        Animations.push_back(Animation);
        return 1;
    }

    void Manager::SetColorDesired() {
        MHColor DesiredColor(MHRCGUI.GetDesiredColor());
        Menu.SetColor(DesiredColor);
        MHRCGUI.SetGUIColor(DesiredColor);
    }

    void Manager::PlayAnimation(std::string AnimationName) {
        for(auto& CurrentAnimation : Animations) {
            if(CurrentAnimation.GetName() == AnimationName) {
                CurrentAnimation.Play(Menu, MHRCGUI);
            }
        }
    }

    void Manager::UpdateMenu() {
        if(MHRCGUI.SetVisible(Menu.IsOpen())) {
            MHColor DesiredColor(MHRCGUI.GetDesiredColor());
            if(MHRCGUI.GetRainbowEnabled()) {
                PlayAnimation("Rainbow");
            } else {
                SetColorDesired();
            }
        }
    }

    void Manager::GUIThread() {
        AnimationData RainbowAnim("Rainbow");
        RainbowAnim.AddStep(FadeHSV({0.f, 90.f, 90.f}, {360.f, 90.f, 90.f}, 1750));

        AddAnimation(RainbowAnim);

        while(1) {
            UpdateMenu();
        }
    }

    void Manager::StartGUIThread() {
        if(Errors == Errorcode::None){
            std::thread(GUIThread).detach();
        }
    }
}