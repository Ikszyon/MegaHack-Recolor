#pragma once

#include <string>
#include <variant>
#include <vector>
#include <utility>

#include "colorfade.hpp"

namespace MHRecolor {
    class AnimationData {
       private:
        std::string AnimationName;
        std::vector<std::variant<FadeRGB, FadeHSV>> AnimationVector;

       public:
        AnimationData(std::string AnimationName, std::vector<std::variant<FadeRGB, FadeHSV>> AnimationVector = {});

        void Play(MHMenu& MHMenuToPlay, GUI& GUIToPlay);

        AnimationData& AddStep(std::variant<FadeRGB, FadeHSV> Step);

        std::string& GetName();
    };
}