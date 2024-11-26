//
// Created by tang on 11/6/24.
//

#include "Module.hpp"

#include <cassert>
#include <functional>

#include "GlobalString.hpp"

namespace llvm {
    Module::Module() {
        _context = std::make_shared<LLVMContext>();
    }

    LLVMContextPtr Module::context() {
        return _context;
    }

    void Module::addFunction(FunctionPtr function) {
        // CANNOT add a null function inside;
        assert(function != nullptr);
        _functions.push_back(function);
    }

    void Module::addGlobalVariable(GlobalVariablePtr gvp) {
        assert(gvp !=  nullptr);
        _globalVariables.push_back(gvp);
    }

    void Module::addString(GlobalStringPtr str) {
        _strings.push_back(str);
    }

    void Module::print(std::ostream& out) {
        auto sourceName = _context->getSourceName();
        out << "; ModuleID = '" + sourceName + "'" << std::endl;
        out << "source_filename = \"" + sourceName + "\"" << std::endl;
        out << std::endl;

        out << "declare i32 @getint()" << std::endl;
        out << "declare i32 @getchar()" << std::endl;
        out << "declare void @putint(i32)" << std::endl;
        out << "declare void @putch(i32)" << std::endl;
        out << "declare void @putstr(i8*)" << std::endl;
        out << std::endl;

        // GlobalStrings
        for (auto& str: _strings) {
            str->print(out);
        }

        // GlobalVaraibles
        for (auto& gvp: _globalVariables) {
            gvp->print(out);
        }
        out << std::endl;

        // Function
        for (auto fp: _functions) {
            fp->print(out);
        }
    }

    // TODO:  analyze the active variables in every Function
    void Module::analyzeActiveVariable() {
        for (auto& f: _functions) {
            f->analizeActiveVariable();
        }
    }
}
