//
// Created by tang on 11/26/24.
//

#ifndef VARTABLE_HPP
#define VARTABLE_HPP

#include <map>
#include <set>

#include "MipsCommon.hpp"
#include "IR/Common.hpp"
#include "IR/Instructions.hpp"

namespace mips {
    class Variable {
        // llvm
        llvm::ValuePtr _llvmVal;

        // storage
        MipsLocationPtr _location;

    public:
        static VariablePtr New(llvm::ValuePtr inst);
        explicit Variable(llvm::ValuePtr& inst);
        int size();
        bool hasLocation();
        void setLocation(StackSlotPtr slot);
        MipsLocationPtr getLocation() { return _location; }
        MipsRegPtr getTargetReg();
    };

    class VarTable {
    private:
        std::map<llvm::ValuePtr, VariablePtr> _vars;
        std::map<llvm::GlobalValuePtr, VariablePtr> _globalVals;
    public:
        static VarTablePtr New(llvm::ModulePtr module);
        explicit VarTable(llvm::ModulePtr& module);

        VariablePtr findVar(llvm::ValuePtr val);
        std::map<llvm::ValuePtr, VariablePtr>::iterator varBegin();
        std::map<llvm::ValuePtr, VariablePtr>::iterator varEnd();
        std::map<llvm::GlobalValuePtr, VariablePtr>::iterator globalValBegin();
        std::map<llvm::GlobalValuePtr, VariablePtr>::iterator globalValEnd();
    private:
        void addVariable(llvm::ValuePtr val, VariablePtr var);
    };
}

#endif //VARTABLE_HPP
