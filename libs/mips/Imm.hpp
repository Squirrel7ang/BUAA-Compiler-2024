//
// Created by tang on 11/26/24.
//

#ifndef MIPSDATA_HPP
#define MIPSDATA_HPP

#include "MipsCommon.hpp"

namespace mips {
    class MipsData {
        int _val;
        // bool _signed;
    public:
        static MipsDataPtr New(int value);
        void print(std::ostream& out) const;
    private:
        explicit MipsData(int value)
            : _val(value) { }
    };

    static const MipsDataPtr DATA_ZERO = MipsData::New(0);
}

#endif //MIPSDATA_HPP
