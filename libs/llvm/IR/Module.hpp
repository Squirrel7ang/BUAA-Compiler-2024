//
// Created by tang on 11/6/24.
//

#ifndef MODULE_HPP
#define MODULE_HPP

#include <memory>
#include <vector>

#include "Common.hpp"
#include "LLVMContext.hpp"

namespace llvm {
    /**
     * The entire CompUnit's LLVM data structure is Module.
     * Module and Value class is controlled by a shared_ptr,
     * while any other classes is controlled with new/delete
     * options.
     *
     * Hierarchy: Module->Function->BasicBlock->Instruction
     */
    class Module {
        LLVMContextPtr _context;
        std::vector<FunctionPtr> _functions;
        std::vector<GlobalVariablePtr> _globalVariables;
    public:
        explicit Module();
        LLVMContextPtr context();
        LLVMContextPtr newContext();

        // 'functions' related functions
        void addFunction(FunctionPtr);
        void addGlobalVariable(GlobalVariablePtr);
    };
    using ModulePtr = std::shared_ptr<Module>;
}

#endif // MODULE_HPP
