#include "mhmenu.hpp"

namespace MHRecolor {
    Errorcode MHMenu::Init(MHInfo& InitInfo) {
        Info = &InitInfo;
        if(Info == nullptr) {
            return Errorcode::MHInfoIsNullptr;
        }
        return Errorcode::None;
    }

    void MHMenu::SetColor(MHColor Color) {
        Info->SetMenuColor(Color);
        Info->SetMenuColorT(Color * OverlayColor);
    }

    bool MHMenu::IsOpen() {
        return Info->GetMenuState();
    }
}