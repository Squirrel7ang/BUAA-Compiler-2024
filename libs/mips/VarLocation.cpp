//
// Created by tang on 12/6/24.
//

#include "VarLocation.hpp"

namespace mips {
    VarLocation::VarLocation(VarLocationID id)
        : _vlid(id) { }

    VarLocation::VarLocation(VarLocationID id, unsigned int offset)
        : _vlid(id), _offset(offset) {
    }
} // mips