//
// Created by tang on 11/7/24.
//

#ifndef GLOBALVARIABLE_HPP
#define GLOBALVARIABLE_HPP

#include <memory>

#include "GlobalValue.hpp"
#include "LLVMContext.hpp"
#include "Type.hpp"
#include "ConstantData.hpp"

namespace llvm {
    class GlobalVariable : public GlobalValue {
    private:
        ConstantDataPtr _initVal;
        bool _isConst;
        TypePtr _type;
        std::string _name;
    public:
        explicit GlobalVariable(LLVMContextPtr& context, TypePtr ty,
                                ConstantDataPtr initVal, bool constant, std::string& name)
                : GlobalValue(context, ty, GLOBAL_VARIABLE_T), _initVal(initVal),
                  _isConst(constant), _name(name) { }
        void print(std::ostream& out) {
            out << "@" << _name << " = dso_local";
            out << (_isConst ? " constant " : " global ");
            _type->print(out);
            out << " ";
            _initVal->print(out);
        }
        void printRef(std::ostream& out) {
            out << "@" << _name;
        }
    };
    using GlobalVariablePtr = std::shared_ptr<GlobalVariable>;
}

#endif //GLOBALVARIABLE_HPP
