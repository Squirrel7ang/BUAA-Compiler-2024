//
// Created by tang on 11/7/24.
//

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

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
        void print(std::ostream& out) {
            out << "define dso_local ";
            auto funcTypePtr = std::dynamic_pointer_cast<FunctionType>(_type);
            funcTypePtr->_retType->print(out);
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
    };
}

#endif //FUNCTION_HPP
