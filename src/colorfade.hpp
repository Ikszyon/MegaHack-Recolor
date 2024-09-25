#pragma once

#include "mhcolor.hpp"
#include "mhmenu.hpp"
#include "gui.hpp"

namespace MHRecolor {
    class FadeRGB {
      private:
        MHColor m_Color1;
        MHColor m_Color2;

        int m_DiffR;
        int m_DiffG;
        int m_DiffB;
        int m_DiffMax;  // == number of animation steps

        float m_IncR;
        float m_IncG;
        float m_IncB;

        int m_Duration;

      public:
        FadeRGB(MHColor Color1, MHColor Color2, int DurationMS);

        void Play(MHMenu& MHMenuToPlay, GUI& GUIToPlay);

        MHColor GetLastColor();
    };

    class FadeHSV {
      private:
        MHColorHSV m_Color1;
        MHColorHSV m_Color2;

        float m_DiffH;
        float m_DiffS;
        float m_DiffV;
        float m_DiffMax;  // == number of animation steps

        float m_IncH;
        float m_IncS;
        float m_IncV;

        int m_Duration;

      public:
        FadeHSV(MHColorHSV Color1, MHColorHSV Color2, int DurationMS);

        void Play(MHMenu& MHMenuToPlay, GUI& GUIToPlay);

        MHColorHSV GetLastColor();
    };
}