#include "savedata.hpp"

bool matjson::Serialize<MHRecolor::SaveData>::is_json(matjson::Value const& Value) {
    return (Value.contains("MenuColor") &&
            Value.contains("RainbowEnabled") &&
            Value["MenuColor"]["R"].is_number() &&
            Value["MenuColor"]["G"].is_number() &&
            Value["MenuColor"]["B"].is_number() &&
            Value["RainbowEnabled"].is_bool());
}

MHRecolor::SaveData matjson::Serialize<MHRecolor::SaveData>::from_json(matjson::Value const& Value) {
    return MHRecolor::SaveData(
        {static_cast<unsigned char>(Value["MenuColor"]["R"].as_int()),
         static_cast<unsigned char>(Value["MenuColor"]["G"].as_int()),
         static_cast<unsigned char>(Value["MenuColor"]["B"].as_int())},
        Value["RainbowEnabled"].as_bool());
}

matjson::Value matjson::Serialize<MHRecolor::SaveData>::to_json(MHRecolor::SaveData const& Value) {
    auto Obj = matjson::Object();
    Obj["MenuColor"]["R"] = static_cast<unsigned int>(Value.MenuColor.r);
    Obj["MenuColor"]["G"] = static_cast<unsigned int>(Value.MenuColor.g);
    Obj["MenuColor"]["B"] = static_cast<unsigned int>(Value.MenuColor.b);
    Obj["RainbowEnabled"] = Value.RainbowEnabled;
    return Obj;
}