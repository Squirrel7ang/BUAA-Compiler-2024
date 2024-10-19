//
// Created by tang on 10/19/24.
//

#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "ast.hpp"
#include "libs/llvm/codeGen.hpp"
#include "libs/llvm/Symbol.hpp"

namespace tang {
    using namespace llvm;
    class Visitor {
        CodeGen _codeGen;
        u_ptr<CompUnit> _compUnit;
    public:
        explicit Visitor(u_ptr<CompUnit>& compUnit, const CodeGen& codeGen)
            : _codeGen(codeGen), _compUnit(std::move(compUnit)) { }
        void visit();
    private:
        void _visitCompUnit();
        void _visitFuncDef();
        void _visitDecl();
        void _visitMainFuncDef();
    };
} // namespace tang

#endif //VISITOR_HPP
