//
// Created by tang on 12/6/24.
//

#ifndef VARLOCATION_HPP
#define VARLOCATION_HPP

namespace mips {
    enum VarLocationID {
        VLID_REG,
        VLID_STACK,
    };
    class VarLocation {
    private:
        VarLocationID _vlid;
    public:
        virtual ~VarLocation() = default;

        explicit VarLocation(VarLocationID id);
        virtual bool is(VarLocationID varLocID) {
            return _vlid == varLocID;
        }
    };
} // mips

#endif //VARLOCATION_HPP
