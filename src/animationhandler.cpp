#include "animationhandler.hpp"

namespace MHRecolor {
    AnimationData::AnimationData(std::string AnimationName, std::vector<std::variant<FadeRGB, FadeHSV>> AnimationVector)
        : AnimationName(AnimationName), AnimationVector(AnimationVector) {}

    template<class... Types>
    struct Overload : Types... {
        using Types::operator()...;
    };
    void AnimationData::Play(MHMenu& MHMenuToPlay, GUI& GUIToPlay) {
        for(auto& CurrentFade : AnimationVector) {
            std::visit(Overload{
                            [&](FadeHSV& Animation) {
                               Animation.Play(MHMenuToPlay, GUIToPlay);
                            },
                            [&](FadeRGB& Animation) {
                               Animation.Play(MHMenuToPlay, GUIToPlay);
                            }},
                        CurrentFade);
        }
    }

    AnimationData& AnimationData::AddStep(std::variant<FadeRGB, FadeHSV> Step) {
        AnimationVector.push_back(Step);
        return *this;
    }

    std::string& AnimationData::GetName() {
        return AnimationName;
    }
}