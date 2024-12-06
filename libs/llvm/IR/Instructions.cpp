//
// Created by tang on 11/6/24.
//

#include "Instructions.hpp"

namespace llvm {
    int AllocaInst::allocateSpace() const {
        return _allocaType->getSize();
    }

    void CallInst::print(std::ostream& out) {
        if (!_type->isVoidTy()) {
            printRef(out);
            out << " = ";
        }
        out << "call ";
        _type->print(out);
        out << " ";
        _func->printRef(out);
        out << "(";

        // parameters
        for (int i = 0; i < _usees.size(); i++) {
            getUsee(i)->printRefWithType(out);
            if (i < _usees.size() - 1) {
                out << ", ";
            }
        }

        out << ")";
    }

}
