//
// Created by tang on 11/6/24.
//

#include "Function.hpp"

namespace llvm {

    vector<BasicBlockPtr>::iterator Function::blockBegin() {
        return _blocks.begin();
    }

    vector<BasicBlockPtr>::iterator Function::blockEnd() {
        return _blocks.end();
    }

    vector<ArgumentPtr>::iterator Function::argumentBegin() {
        return _args.begin();
    }

    vector<ArgumentPtr>::iterator Function::argumentEnd() {
        return _args.end();
    }

    void Function::addBasicBlock(BasicBlockPtr block) {
        _blocks.push_back(block);
    }

    void Function::clearEmptyBasicBlocks() {
        for (auto it = _blocks.begin(); it != _blocks.end();) {
            if ((*it)->isEmptyBlock())
                it = _blocks.erase(it);
            else
                ++it;
        }
    }

    // TODO: calculate every in and out for each block
    void Function::analyzeActiveVariable() {
        // calculate the use and def of each basicBlock;
        for (auto& block: _blocks) {
            block->calUseDef();
        }

        // TODO: do I need to add arg to the In of the first block?

        bool changed = true;
        while (changed) {
            changed = false;
            for (auto& block: _blocks) {
                changed |= block->calVarOut();
                changed |= block->calVarIn();
            }
        }

        // insert cross block active Variables into GlobalVariables
        for (auto& block: _blocks) {
            auto& vars = block->getVarOut();
            for (auto& var: vars) {
                globalVariables.insert(var);
            }
        }

        // calculate conflict variables
        calConflictVars();
    }

    /**
     * calculate the total space this function is going
     * to use in bytes.
     * @return total bytes this Function is going to use
     */
    int Function::spaceUse() {
        int ret = 0;
        for (auto& block: _blocks) {
            ret += block->calSpaceUse();
        }
        return ret;
    }

    void Function::calConflictVars() {
        for (auto& block: _blocks) {
            calConflictVars(block);
        }
    }

    void Function::calConflictVars(BasicBlockPtr block) {
        auto&& varOut = block->getVarOut();
        auto&& varDef = block->getVarDef();

        for (auto i = varOut.begin(); i != varOut.end(); ++i) {
            auto& inst0 = *i;
            // variables in Out are conflict with each other
            for (auto j = std::next(i); j != varOut.end(); ++j) {
                auto& inst1 = *j;
                auto conflictEdge = std::pair(inst0, inst1);
                conflictEdges.push_back(conflictEdge);
            }
            // out is conflict with def
            for (const auto& inst1 : varDef) {
                auto conflictEdge = std::pair(inst0, inst1);
                conflictEdges.push_back(conflictEdge);
            }
        }
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
        for (const auto& b: _blocks) {
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