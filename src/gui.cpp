#include "gui.hpp"

#define ColToImVec(R, G, B, A) ImVec4(R / 255.f, G / 255.f, B / 255.f, A / 255.f)
#define ImVecToRGB(Vec) MHColor(Vec.x * 255.f, Vec[1] * 255.f, Vec[2] * 255.f)

namespace MHRecolor {
    bool GUI::IsInitialized = 0;
    float GUI::DesiredColor[3] = {1.f, 1.f, 1.f};
    ImVec4* GUI::GUIColors = 0;
    bool GUI::RainbowEnabled = 0;

    Errorcode GUI::Init() {
        if(!IsInitialized) {
            ImGuiCocos::get()
                .setup([&] {
                    auto* Font = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "Muli.ttf").string().c_str(), 22.0f);

                    auto &Style = ImGui::GetStyle();
                    Style.WindowPadding = ImVec2(5, 4);
                    Style.WindowRounding = 0.f;
                    Style.FramePadding = ImVec2(8, 4);
                    Style.FrameRounding = 0.f;
                    Style.PopupRounding = 0.f;
                    Style.ItemSpacing = ImVec2(12, 3);
                    Style.ItemInnerSpacing = ImVec2(8, 6);
                    Style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
                    Style.IndentSpacing = 25.0f;
                    Style.ScrollbarSize = 15.0f;
                    Style.ScrollbarRounding = 9.0f;
                    Style.GrabMinSize = 5.0f;
                    Style.GrabRounding = 3.0f;
                    Style.WindowBorderSize = 0.f;
                    Style.WindowMinSize = ImVec2(32, 32);
                    Style.DisplayWindowPadding = ImVec2(0, 0);
                    Style.WindowMenuButtonPosition = ImGuiDir_Left;

                    GUIColors = Style.Colors;

                    GUIColors[ImGuiCol_Text] = ColToImVec(0xEB, 0xEB, 0xE3, 0xFF);
                    GUIColors[ImGuiCol_TextDisabled] = ColToImVec(0xEB, 0xEB, 0xE3, 0x7F);

                    GUIColors[ImGuiCol_WindowBg] = ColToImVec(0x2A, 0x2A, 0x2A, 0xFF);

                    GUIColors[ImGuiCol_TitleBg] =
                        GUIColors[ImGuiCol_TitleBgActive] =
                            GUIColors[ImGuiCol_TitleBgCollapsed] =
                                ColToImVec(0xAD, 0x62, 0xEE, 0xFF);

                    GUIColors[ImGuiCol_Button] = ColToImVec(0x2A, 0x2A, 0x2A, 0xFF);
                    GUIColors[ImGuiCol_ButtonHovered] = ColToImVec(0x1B, 0x1B, 0x1B, 0xFF);
                    GUIColors[ImGuiCol_ButtonActive] = ColToImVec(0x0F, 0x0F, 0x0F, 0xFF);

                    GUIColors[ImGuiCol_FrameBg] = ColToImVec(0x1B, 0x1B, 0x1B, 0xFF);
                    GUIColors[ImGuiCol_FrameBgHovered] = ColToImVec(0x33, 0x33, 0x33, 0xFF);
                    GUIColors[ImGuiCol_FrameBgActive] = ColToImVec(0x1E, 0x1E, 0x1E, 0xFF);

                    GUIColors[ImGuiCol_ScrollbarBg] = ColToImVec(0x1B, 0x1B, 0x1B, 0xFF);
                    GUIColors[ImGuiCol_ScrollbarGrab] = ColToImVec(0x64, 0x64, 0x64, 0xFF);
                    GUIColors[ImGuiCol_ScrollbarGrabHovered] = ColToImVec(0x33, 0x33, 0x33, 0xFF);
                    GUIColors[ImGuiCol_ScrollbarGrabActive] = ColToImVec(0x1E, 0x1E, 0x1E, 0xFF);

                    GUIColors[ImGuiCol_CheckMark] = ColToImVec(0xAD, 0x62, 0xEE, 0xFF);
                    GUIColors[ImGuiCol_SliderGrab] = ColToImVec(0x0F, 0x0F, 0x0F, 0xFF);
                    GUIColors[ImGuiCol_SliderGrabActive] = ColToImVec(0xAD, 0x62, 0xEE, 0xFF);

                    GUIColors[ImGuiCol_Border] = ColToImVec(0x00, 0x00, 0x00, 0x00);
                    GUIColors[ImGuiCol_BorderShadow] = ColToImVec(0x00, 0x00, 0x00, 0x00);

                    GUIColors[ImGuiCol_PopupBg] = ColToImVec(0x2A, 0x2A, 0x2A, 0xFF);
                    GUIColors[ImGuiCol_Header] = ColToImVec(0xAD, 0x62, 0xEE, 0xFF);
                    GUIColors[ImGuiCol_HeaderHovered] = ColToImVec(0xAD, 0x62, 0xEE, 0xFF);
                    GUIColors[ImGuiCol_HeaderActive] = ColToImVec(0xAD, 0x62, 0xEE, 0xFF);
                })
                .draw([&] {
                    ImGui::Begin("MH Recolor");
                    ImGui::ColorEdit3("Menu Color", DesiredColor);
                    ImGui::Checkbox("Rainbow", &RainbowEnabled);
                    ImGui::End();
                })
                .setVisible(0);
        }
        IsInitialized = 1;
        return Errorcode::None;
    }

    bool GUI::SetVisible(bool SetVisibility) {
        if(LastVisibility != SetVisibility) {
            ImGuiCocos::get().setVisible(SetVisibility);
            LastVisibility = SetVisibility;
        }
        return SetVisibility;
    }

    void GUI::SetGUIColor(MHColor Color) {
        GUIColors[ImGuiCol_CheckMark] =
            GUIColors[ImGuiCol_SliderGrabActive] =
                GUIColors[ImGuiCol_HeaderHovered] =
                    GUIColors[ImGuiCol_HeaderActive] =
                        GUIColors[ImGuiCol_TitleBg] =
                            GUIColors[ImGuiCol_TitleBgActive] =
                                GUIColors[ImGuiCol_TitleBgCollapsed] =
                                    ColToImVec(Color.r, Color.g, Color.b, 0xFF);
    }

    MHColor GUI::GetDesiredColor() {
        return {
            static_cast<unsigned char>(std::round(DesiredColor[0] * 255.f)),
            static_cast<unsigned char>(std::round(DesiredColor[1] * 255.f)),
            static_cast<unsigned char>(std::round(DesiredColor[2] * 255.f))
        };
    }

    void GUI::SetDesiredColor(MHColor Color) {
        DesiredColor[0] = static_cast<float>(Color.r / 255.f);
        DesiredColor[1] = static_cast<float>(Color.g / 255.f);
        DesiredColor[2] = static_cast<float>(Color.b / 255.f);
    }

    bool GUI::GetRainbowEnabled() {
        return RainbowEnabled;
    }

    void GUI::SetRainbowEnabled(bool Bool) {
        RainbowEnabled = Bool;
    }
}