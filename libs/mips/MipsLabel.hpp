//
// Created by tang on 12/11/24.
//

#ifndef MIPSLABEL_HPP
#define MIPSLABEL_HPP

#include <string>
#include <map>

#include "MipsImm.hpp"
#include "IR/Common.hpp"

namespace mips {
    class MipsLabel: public MipsImm {
        std::string _name;
    public:
        static MipsLabelPtr New(std::string& name);
        explicit MipsLabel(std::string& name);
        void print(std::ostream& out) const override;
    };


    class LabelTable {
    private:
        std::map<llvm::BasicBlockPtr, MipsLabelPtr> _table;
    public:
        static LabelTablePtr New(llvm::ModulePtr);
        explicit LabelTable(llvm::ModulePtr);

        MipsLabelPtr findLabel(llvm::BasicBlockPtr);
        std::map<llvm::BasicBlockPtr, MipsLabelPtr>::iterator begin();
        std::map<llvm::BasicBlockPtr, MipsLabelPtr>::iterator end();
        int size() { return _table.size(); }
    private:
        void addLabel(llvm::BasicBlockPtr, MipsLabelPtr);
    };
}

#endif //MIPSLABEL_HPP
