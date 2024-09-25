#pragma once

namespace MHRecolor {
    enum Errorcode : int {
        None             = 0 << 0,
        MHNotFound       = 1 << 0,
        MHDLLInfoError   = 1 << 1,
        MHSigsNotFound   = 1 << 2,
        MHVarsNotFound   = 1 << 3,
        MHInfoIsNullptr  = 1 << 4
    };

    Errorcode operator|(Errorcode A, Errorcode B);
    Errorcode& operator|=(Errorcode& A, Errorcode B);
}