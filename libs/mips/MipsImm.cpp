//
// Created by tang on 11/26/24.
//

#include "MipsImm.hpp"

namespace mips {
    MipsDataPtr MipsImm::New(int value) {
        return std::make_shared<MipsImm>(value);
    }

    void MipsImm::print(std::ostream &out) const {
        out << _val;
    }

    MipsImm::MipsImm(int value)
        : _val(value) { }
}
