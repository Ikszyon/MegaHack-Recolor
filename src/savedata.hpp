#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include "mhcolor.hpp"

namespace MHRecolor {
    struct SaveData {
        MHColor MenuColor = {0x00, 0x00, 0x00};
        bool RainbowEnabled = 0;

        SaveData() = default;
        SaveData(MHColor MenuColor, bool RainbowEnabled)
            : MenuColor(MenuColor), RainbowEnabled(RainbowEnabled) {}
    };
}

template<>
struct matjson::Serialize<MHRecolor::SaveData> {
    static bool is_json(matjson::Value const& Value);
    static MHRecolor::SaveData from_json(matjson::Value const& Value);
    static matjson::Value to_json(MHRecolor::SaveData const& Value);
};















struct MyCustomSaveData {
    int x;
    int y;
};

template<>
struct matjson::Serialize<MyCustomSaveData> {
    static bool is_json(matjson::Value const& Value) {
        return Value.is_object();
    }

    static MyCustomSaveData from_json(matjson::Value const& value) {
        return MyCustomSaveData {
            .x = value["x"].as_int(),
            .y = value["y"].as_int()
        };
    }

    static matjson::Value to_json(MyCustomSaveData const& value) {
        auto obj = matjson::Object();
        obj["x"] = value.x;
        obj["y"] = value.y;
        return obj;
    }
};