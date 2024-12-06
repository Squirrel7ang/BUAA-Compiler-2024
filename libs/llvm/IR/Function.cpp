//
// Created by tang on 11/6/24.
//

#include "Function.hpp"

namespace llvm {
    void Function::clearEmptyBasicBlocks() {
        for (auto it = _blocks.begin(); it != _blocks.end();) {
            if ((*it)->isEmptyBlock())
                it = _blocks.erase(it);
            else
                ++it;
        }
    }

    // TODO: calculate every in and out for each block
    void Function::analizeActiveVariable() {
        // calculate the use and def of each basicBlock;
        for (auto& block: _blocks) {
            block->calUseDef();
        }

        // add arguments to the first basic block
        for (auto& arg: _args) {
            _blocks.front()->addVarIn(arg);
        }

        bool changed = true;
        while (changed) {
            changed = false;
            for (auto& block: _blocks) {
                changed |= block->calVarOut();
                changed |= block->calVarIn();
            }
        }
    }

    /**
     * calculate the total space this function is going
     * to use in bytes.
     * @return total bytes this Function is going to use
     */
    int Function::calSpaceUse() {
        int ret = 0;
        for (auto& block: _blocks) {
            ret += block->calSpaceUse();
        }
        return ret;
    }

    void Function::printRef(std::ostream &out) {
        out << '@' << _name;
    }


    void Function::print(std::ostream& out) {
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

    Function::Function(LLVMContextPtr& context, TypePtr ty,
                      std::vector<ArgumentPtr>& parameters, std::string name)
            : GlobalValue(context, ty, FUNCTION_T), _args(parameters), _name(name) { }

    Function::Function(LLVMContextPtr& context, TypePtr ty,
                      std::vector<TypePtr>& parameters, std::string name)
            : GlobalValue(context, ty, FUNCTION_T), _name(name) {
        for (auto para: parameters) {
            auto argPtr = std::make_shared<Argument>(context, para);
            _args.push_back(argPtr);
        }
    }

   Function::Function(LLVMContextPtr& context, TypePtr ty, std::string name)
            : GlobalValue(context, ty, FUNCTION_T), _name(name) { }

    void Function::setIndex() {
        int index = 0;
        for (auto& arg: _args) {
            arg->setIndex(index);
        }
        for (auto& block: _blocks) {
            block->setIndex(index);
        }
    }
}