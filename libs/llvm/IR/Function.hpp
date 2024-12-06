//
// Created by tang on 11/7/24.
//

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "Common.hpp"
#include "Argument.hpp"
#include "BasicBlock.hpp"
#include "GlobalValue.hpp"
#include "Type.hpp"

namespace llvm {
    /**
     * The TypePtr ty for a Function is the type of its return Value,
     * most likely not a FUNCTION_T.
     *
     * while the type for the symbol with the name of the function is
     * the type of FUNCTION_T.
     */
    class Function : public GlobalValue {
    private:
        vector<BasicBlockPtr> _blocks;
        vector<ArgumentPtr> _args;
        std::string _name;
        std::set<InstructionPtr> beginOut;
        std::set<InstructionPtr> beginIn;
        std::set<InstructionPtr> exitIn;
        std::set<InstructionPtr> exitOut;

        // crossBlockVar
        std::set<ValuePtr> globalVariables;
        ConflictEdges conflictEdges;
    public:
        explicit Function(LLVMContextPtr& context, TypePtr ty,
                          std::vector<ArgumentPtr>& parameters, std::string name);
        explicit Function(LLVMContextPtr& context, TypePtr ty,
                          std::vector<TypePtr>& parameters, std::string name);
        explicit Function(LLVMContextPtr& context, TypePtr ty, std::string name);

        // iterator for basicBlocks and arguments inside;
        vector<BasicBlockPtr>::iterator blockBegin();
        vector<BasicBlockPtr>::iterator blockEnd();
        vector<ArgumentPtr>::iterator argumentBegin();
        vector<ArgumentPtr>::iterator argumentEnd();
        void addBasicBlock(BasicBlockPtr block);
        int argNum() { return _args.size(); }
        ArgumentPtr getArg(int i) { return _args.at(i); }
        void setIndex();
        void print(std::ostream& out) override;
        void printRef(std::ostream& out) override;

        // dataflow and optimization
        void clearEmptyBasicBlocks();
        void analyzeActiveVariable();
        int spaceUse();
        std::set<ValuePtr>& getGlobalVars() { return globalVariables; }
        ConflictEdges& getConflictVars() { return conflictEdges; }
    private:
        void calConflictVars();
        void calConflictVars(BasicBlockPtr block);
    };
}

#endif //FUNCTION_HPP
