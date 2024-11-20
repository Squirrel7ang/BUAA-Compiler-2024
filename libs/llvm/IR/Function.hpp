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
    public:
        explicit Function(LLVMContextPtr& context, TypePtr ty,
                          std::vector<ArgumentPtr>& parameters, std::string name)
                : GlobalValue(context, ty, FUNCTION_T), _args(parameters), _name(name) { }
        explicit Function(LLVMContextPtr& context, TypePtr ty,
                          std::vector<TypePtr>& parameters, std::string name)
                : GlobalValue(context, ty, FUNCTION_T), _name(name) {
            for (auto para: parameters) {
                auto argPtr = std::make_shared<Argument>(context, para);
                _args.push_back(argPtr);
            }
        }
        explicit Function(LLVMContextPtr& context, TypePtr ty,
                          std::string name)
                : GlobalValue(context, ty, FUNCTION_T), _name(name) { }
        void addBasicBlock(BasicBlockPtr block) {
            _blocks.push_back(block);
        }
        int argNum() { return _args.size(); }
        ArgumentPtr getArg(int i) { return _args.at(i); }
        void setIndex() {
            int index = 0;
            for (auto& arg: _args) {
                arg->setIndex(index);
            }
            for (auto& block: _blocks) {
                block->setIndex(index);
            }
        }
        void print(std::ostream& out) {
            // TODO: index temporary variables
            setIndex();

            out << "define dso_local ";
            _type->print(out);
            out << " @" << _name << '(';
            for (int i = 0; i < _args.size(); i++) {
                _args.at(i)->print(out);
                // TODO: print Argument's tmpVar's Index
                if (i != _args.size() - 1) {
                    out << ", ";
                }
            }
            out << ") {" << std::endl;

            // print basicBlocks
            for (auto b: _blocks) {
                b->print(out);
            }

            out << "}" << std::endl << std::endl;
        }
        void printRef(std::ostream& out) override {
            out << "@" << _name;
        }
    };
}

#endif //FUNCTION_HPP
