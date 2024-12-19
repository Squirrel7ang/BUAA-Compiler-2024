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
        static MipsLabelPtr New(const std::string& name);
        explicit MipsLabel(const std::string& name);
        void print(std::ostream& out) const override;
    };



    class LabelTable {
    private:
        std::map<llvm::BasicBlockPtr, MipsLabelPtr> _basicBlockTable;
        std::map<llvm::FunctionPtr, MipsLabelPtr> _funcTable;
    public:
        static LabelTablePtr New(llvm::ModulePtr);
        explicit LabelTable(llvm::ModulePtr);

        MipsLabelPtr findLabel(llvm::BasicBlockPtr);
        MipsLabelPtr findLabel(llvm::FunctionPtr);
        std::map<llvm::BasicBlockPtr, MipsLabelPtr>::iterator begin();
        std::map<llvm::BasicBlockPtr, MipsLabelPtr>::iterator end();
    private:
        void addLabel(llvm::BasicBlockPtr, MipsLabelPtr);
    };
}

#endif //MIPSLABEL_HPP
