#pragma once

#include "mhinfo.hpp"
#include "mhcolor.hpp"
#include "errorcodes.hpp"

namespace MHRecolor {
    class MHMenu {
       private:
        MHInfo* Info = nullptr;

        // Color that gets overlayed onto options that are disabled when searching
        MHColor OverlayColor = {0x80, 0x80, 0x80};

       public:
        MHMenu() = default;

        Errorcode Init(MHInfo& Info);

        void SetColor(MHColor Color);

        bool IsOpen();
    };
}