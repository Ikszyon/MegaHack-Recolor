#pragma once

#include "errorcodes.hpp"

namespace MHRecolor {
    Errorcode operator|(Errorcode A, Errorcode B) {
        return static_cast<Errorcode>(static_cast<int>(A) | static_cast<int>(B));
    }

    Errorcode& operator|=(Errorcode& A, Errorcode B) {
        return (A = (A | B));
    }
}