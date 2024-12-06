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
        std::set<ValuePtr> beginOut;
        std::set<ValuePtr> beginIn;
        std::set<ValuePtr> exitIn;
        std::set<ValuePtr> exitOut;
    public:
        explicit Function(LLVMContextPtr& context, TypePtr ty,
                          std::vector<ArgumentPtr>& parameters, std::string name);
        explicit Function(LLVMContextPtr& context, TypePtr ty,
                          std::vector<TypePtr>& parameters, std::string name);
        explicit Function(LLVMContextPtr& context, TypePtr ty, std::string name);

        // iterator for basicBlocks and arguments inside;
        vector<BasicBlockPtr>::iterator blockBegin() {
            return _blocks.begin();
        }
        vector<BasicBlockPtr>::iterator blockEnd() {
            return _blocks.end();
        }
        vector<ArgumentPtr>::iterator argumentBegin() {
            return _args.begin();
        }
        vector<ArgumentPtr>::iterator argumentEnd() {
            return _args.end();
        }

        void addBasicBlock(BasicBlockPtr block) {
            _blocks.push_back(block);
        }
        int argNum() { return _args.size(); }
        ArgumentPtr getArg(int i) { return _args.at(i); }
        void setIndex();
        void print(std::ostream& out) override;
        void printRef(std::ostream& out) override;
        void clearEmptyBasicBlocks();
        void analizeActiveVariable();
        int calSpaceUse();
    };
}

#endif //FUNCTION_HPP
