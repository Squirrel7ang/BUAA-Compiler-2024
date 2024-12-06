//
// Created by tang on 11/6/24.
//

#ifndef MODULE_HPP
#define MODULE_HPP

#include <memory>
#include <vector>

#include "Common.hpp"
#include "Function.hpp"

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
        vector<GlobalVariablePtr> _globalVariables;
        vector<FunctionPtr> _functions;
        vector<GlobalStringPtr> _strings;
    public:
        explicit Module();
        LLVMContextPtr context();
        LLVMContextPtr newContext();

        // 'functions' related functions
        void addFunction(FunctionPtr);
        void addGlobalVariable(GlobalVariablePtr);
        void addString(GlobalStringPtr str);

        // iterator for functions inside;
        vector<FunctionPtr>::iterator functionBegin();
        vector<FunctionPtr>::iterator functionEnd();

        // Printer
        std::string getSourceName();
        void print(std::ostream& out);

        // dataflow
        void clearEmptyBasicBlock();
        void analyzeActiveVariable();
    };
    using ModulePtr = std::shared_ptr<Module>;
}

#endif // MODULE_HPP
