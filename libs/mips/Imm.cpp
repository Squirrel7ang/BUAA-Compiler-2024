//
// Created by tang on 11/26/24.
//

#include "Imm.hpp"

namespace mips {
    MipsDataPtr MipsData::New(int value) {
        return std::make_shared<MipsData>(value);
    }

    void MipsData::print(std::ostream &out) const {
        out << _val;
    }
}
