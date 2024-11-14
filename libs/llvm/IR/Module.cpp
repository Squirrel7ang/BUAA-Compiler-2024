//
// Created by tang on 11/6/24.
//

#include "Module.hpp"

#include <cassert>

namespace llvm {
    Module::Module() {
        _context = std::make_shared<LLVMContext>();
    }

    LLVMContextPtr Module::context() {
        return _context;
    }

    void Module::addFunction(FunctionPtr function) {
        // CANNOT add a null function inside;
        assert(function != nullptr);
        _functions.push_back(function);
    }

    void Module::addGlobalVariable(GlobalVariablePtr gvp) {
        assert(gvp !=  nullptr);
        _globalVariables.push_back(gvp);
    }

}
