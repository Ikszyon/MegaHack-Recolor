#include "colorfade.hpp"

namespace MHRecolor {
    FadeRGB::FadeRGB(MHColor Color1, MHColor Color2, int DurationMS)
        : m_Color1(Color1), m_Color2(Color2), m_Duration(DurationMS) {
        m_DiffR = m_Color2.r - m_Color1.r;
        m_DiffG = m_Color2.g - m_Color1.g;
        m_DiffB = m_Color2.b - m_Color1.b;

        // absolute value of differences
        int AbsDiffR = abs(m_DiffR);
        int AbsDiffG = abs(m_DiffG);
        int AbsDiffB = abs(m_DiffB);

        m_DiffMax = std::max({AbsDiffR, AbsDiffG, AbsDiffB});

        if(!m_DiffMax) {
            return;
        }

        m_IncR = static_cast<float>(m_DiffR) / m_DiffMax;
        m_IncG = static_cast<float>(m_DiffG) / m_DiffMax;
        m_IncB = static_cast<float>(m_DiffB) / m_DiffMax;
    }

    void FadeRGB::Play(MHMenu& MHMenuToPlay, GUI& GUIToPlay) {
        if(!m_DiffMax) {
            return;
        }

        MHColor CurrentColor;

        float SleepDuration = m_Duration < m_DiffMax ? 1.f : m_Duration / m_DiffMax;
        unsigned char ValueR = m_Color1.r;
        unsigned char ValueG = m_Color1.g;
        unsigned char ValueB = m_Color1.b;

        for(int i = 0; i < (m_DiffMax - 1); i++) {
            if(!MHMenuToPlay.IsOpen() || !GUIToPlay.GetRainbowEnabled()) {
                return;
            }

            ValueR += m_IncR;
            ValueG += m_IncG;
            ValueB += m_IncB;

            CurrentColor = {static_cast<unsigned char>(ValueR), static_cast<unsigned char>(ValueG), static_cast<unsigned char>(ValueB)};

            MHMenuToPlay.SetColor(CurrentColor);
            GUIToPlay.SetGUIColor(CurrentColor);

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(SleepDuration)));
        }
    }

    FadeHSV::FadeHSV(MHColorHSV Color1, MHColorHSV Color2, int DurationMS)
        : m_Color1(Color1), m_Color2(Color2), m_Duration(DurationMS) {
        m_DiffH = m_Color2.H - m_Color1.H;
        m_DiffS = m_Color2.S - m_Color1.S;
        m_DiffV = m_Color2.V - m_Color1.V;

        // absolute value of differences
        float AbsDiffH = abs(m_DiffH);
        float AbsDiffS = abs(m_DiffS);
        float AbsDiffV = abs(m_DiffV);

        m_DiffMax = std::max({AbsDiffH, AbsDiffS, AbsDiffV});

        if(!m_DiffMax) {
            return;
        }

        m_IncH = m_DiffH / m_DiffMax;
        m_IncS = m_DiffS / m_DiffMax;
        m_IncV = m_DiffV / m_DiffMax;
    }

    void FadeHSV::Play(MHMenu& MHMenuToPlay, GUI& GUIToPlay) {
        if(!m_DiffMax) {
            return;
        }

        MHColor CurrentColor;
        float SleepDuration = m_Duration < m_DiffMax ? m_DiffMax : m_Duration / m_DiffMax;
        float ValueH = m_Color1.H;
        float ValueS = m_Color1.S;
        float ValueV = m_Color1.V;

        for(float i = 0.f; i < (m_DiffMax - 1.f); i += 1.f) {
            if(!MHMenuToPlay.IsOpen() || !GUIToPlay.GetRainbowEnabled()) {
                return;
            }

            ValueH += m_IncH;
            ValueS += m_IncS;
            ValueV += m_IncV;

            CurrentColor = HSVToRGB({ValueH, ValueS, ValueV});

            MHMenuToPlay.SetColor(CurrentColor);
            GUIToPlay.SetGUIColor(CurrentColor);

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(SleepDuration)));
        }
    }

    MHColor FadeRGB::GetLastColor() {
        return m_Color2;
    }

    MHColorHSV FadeHSV::GetLastColor() {
        return m_Color2;
    }
}