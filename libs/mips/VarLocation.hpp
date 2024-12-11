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
        unsigned int _offset;
    public:
        virtual ~VarLocation() = default;

        explicit VarLocation(VarLocationID id);
        explicit VarLocation(VarLocationID id, unsigned int offset);
        unsigned int getInitOffset() { return _offset; }
        virtual bool is(VarLocationID varLocID) {
            return _vlid == varLocID;
        }
    };
} // mips

#endif //VARLOCATION_HPP
