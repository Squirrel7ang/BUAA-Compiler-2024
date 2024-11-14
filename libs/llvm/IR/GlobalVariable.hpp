//
// Created by tang on 11/7/24.
//

#ifndef GLOBALVARIABLE_HPP
#define GLOBALVARIABLE_HPP

#include <memory>

#include "GlobalValue.hpp"
#include "LLVMContext.hpp"
#include "Type.hpp"

namespace llvm {
    class GlobalVariable : public GlobalValue {
    private:
        ConstantPtr _initVal;
        bool _isConst;
        TypePtr _type;
        std::string _name;
    public:
        explicit GlobalVariable(LLVMContextPtr context, TypePtr ty,
                                ConstantPtr initVal, bool constant, std::string& name)
                : GlobalValue(context, ty, GLOBAL_VARIABLE_T), _initVal(initVal),
                  _isConst(constant), _name(name) { }
        std::string output() {
            std::string ret;
            ret += '@';
            ret += _name;
            ret += " = dso_local";
            ret += _isConst ? " constant" : " global";
            ret += ' ';
            ret += _type->output();
            ret += ' ';
            ret += _initVal->output();
            return ret;
        }
    };
    using GlobalVariablePtr = std::shared_ptr<GlobalVariable>;
}

#endif //GLOBALVARIABLE_HPP
