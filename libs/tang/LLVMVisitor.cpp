//
// Created by tang on 11/14/24.
//

#include "Visitor.hpp"

namespace tang {
    void Visitor::defineGlobalVariable(Symbol& s) {
        auto context = _modulePtr->context();
        llvm::GlobalVariablePtr gv = s.toGlobalVariable(context);
        _modulePtr->addGlobalVariable(gv);
    }

    void Visitor::defineLocalVariable(u_ptr<VarDef>& node, Symbol& s) {
        auto context = _modulePtr->context();
        llvm::AllocaInstPtr aip = s.toAllocaInst(context);
        _curBlock->addInst(aip);

        // initialize;
        if (node->initVal != nullptr) {
            if (s.getType()->isArray()) {

            }
            else {

            }
        }
    }

    llvm::ValuePtr Visitor::genExpIR(u_ptr<Exp>& node) {
        return genAddExpIR(node->addExp);
    }

    llvm::ValuePtr Visitor::genAddExpIR(u_ptr<AddExp>& node) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst0 = genMulExpIR(node->mulExps.at(0));
        llvm::ValuePtr inst1;

        for (int i = 0; i < node->ops.size(); i++) {
            inst1 = genMulExpIR(node->mulExps.at(i+1));
            if (node->ops.at(i).getType() == TK_PLUS) {
                llvm::BinaryOperatorPtr bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_ADD);
                _curBlock->addInst(bip);
            }
            else if (node->ops.at(i).getType() == TK_MINU) {
                llvm::BinaryOperatorPtr bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_SUB);
                _curBlock->addInst(bip);
            }
            else assert(0);

            inst0 = inst1;
        }
        return inst1;
    }

    llvm::ValuePtr Visitor::genMulExpIR(u_ptr<MulExp>& node) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst0 = genUnaryExpIR(node->unaryExps.at(0));
        llvm::ValuePtr inst1; for (int i = 0; i < node->ops.size(); i++) {
            inst1 = genUnaryExpIR(node->unaryExps.at(i+1));
            if (node->ops.at(i).getType() == TK_MULT) {
                llvm::BinaryOperatorPtr bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_MUL);
                _curBlock->addInst(bip);
            }
            else if (node->ops.at(i).getType() == TK_DIV) {
                llvm::BinaryOperatorPtr bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_DIV);
                _curBlock->addInst(bip);
            }
            else if (node->ops.at(i).getType() == TK_MOD) {
                llvm::BinaryOperatorPtr bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_REM);
                _curBlock->addInst(bip);
            }
            else assert(0);

            inst0 = inst1;
        }
        return inst1;
    }

    llvm::ValuePtr Visitor::genUnaryExpIR(u_ptr<UnaryExp>& node) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst;

        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<FuncCall>>) {
                inst = genFuncCallIR(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<PrimaryExp>>) {
                inst = _visitPrimaryExp(arg);
            }
            else assert(0);
        }, *(node->unaryExp));

        for (auto& op: node->unaryOps) {
            llvm::UnaryOperatorPtr uop;
            if (op->isMinus) {
                uop = std::make_shared<llvm::UnaryOperator>(
                    context, context->I32_TY, inst, llvm::UOID_NEG);
            }
            else if (op->isExc) {
                uop = std::make_shared<llvm::UnaryOperator>(
                    context, context->I32_TY, inst, llvm::UOID_NOT);
            }
            _curBlock->addInst(uop);
        }
    }

    llvm::ValuePtr Visitor::genPrimaryExp(u_ptr<PrimaryExp>& node) {
        llvm::ValuePtr inst;
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<Exp>>) {
                inst = genFuncCallIR(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<Number>>) {
                inst = genNumber(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<Character>>) {
                inst = genCharacter(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<LVal>>) {
                inst = genLVal(arg);
            }
            else assert(0);
        }, *(node->primaryExp));
        return inst;
    }

    llvm::CallInstPtr Visitor::genFuncCallIR(u_ptr<FuncCall>& node) {
        auto context = _modulePtr->context();
        llvm::CallInstPtr cip;
        Symbol s;
        _symbolTable.findSymbolGlobal(s, node->ident->str);
        assert(s.getType()->isFunc());
        auto _func_type= std::dynamic_pointer_cast<FuncSymbolType>(s.getType());
        auto _llvm_type = _func_type->_returnType->toLLVMType(context);

        if (node->funcRParams->exps.empty()) {
            cip = std::make_shared<llvm::CallInst>(context, _llvm_type);
        }
        else {
            vector<llvm::ValuePtr> rargs;
            for (auto exp: node->funcRParams->exps) {
                auto inst = genExpIR(exp);
                rargs.push_back(inst);
            }
            cip = std::make_shared<llvm::CallInst>(context, _llvm_type, rargs);
        }

        _curBlock->addInst(cip);

        return cip;

    }

    llvm::ConstantDataPtr Visitor::genNumberIR(u_ptr<Number>& node) {
        auto context = _modulePtr->context();
        llvm::ConstantDataPtr cdp = std::make_shared<llvm::ConstantData>(
            context, context->I32_TY, node->intConst->val);
        return cdp;
    }

    llvm::ConstantDataPtr Visitor::genCharacterIR(u_ptr<Character>& node) {
        auto context = _modulePtr->context();
        llvm::ConstantDataPtr cdp = std::make_shared<llvm::ConstantData>(
            context, context->I8_TY, node->charConst->ch);
        return cdp;
    }

    llvm::LoadInstPtr Visitor::genLValIR(u_ptr<LVal>& node) {
        auto context = _modulePtr->context();
        Symbol s;
        _symbolTable.findSymbolGlobal(s, node->ident->str);

        llvm::LoadInstPtr lip;
        if (s.getType()->isArray()) {
            if (s.getType()->toRawType() == INT_ARRAY_ST) {
                auto sty = std::static_pointer_cast<IntArrayType>(s.getType());
                llvm::GetElePtrInst gepi = std::make_shared<llvm::GetElePtrInst>(
                    context, sty, );
            }
            else if (s.getType()->toRawType() == CHAR_ARRAY_ST) {
                auto sty = std::static_pointer_cast<CharArrayType>(s.getType());
                llvm::GetElePtrInst gepi = std::make_shared<llvm::GetElePtrInst>(
                    context, sty)
            }

        }
        else {
            lip = std::make_shared<llvm::LoadInst>(
                context, s.getType()->toLLVMType(context), s.getLLVMValue());
            _curBlock->addInst(lip);
        }
        return lip;
    }

    void Visitor::initLocalVariable(Symbol& s, llvm::ValuePtr ptr) {
        auto context = _modulePtr->context();
        assert(!s.getType()->isArray());

        llvm::ConstantDataPtr = std::make_shared<llvm::ConstantData>(context, ptr->getType(), );

        llvm::StoreInstPtr sip = std::make_shared<llvm::StoreInst>(
            context, s.getType()->toLLVMType(context), s.initValAt(0), ptr);
    }

    void Visitor::initLocalArray(Symbol& s) {

    }

    void Visitor::assignLocalVariable(Symbol& s) {

    }

    void Visitor::loadLocalVariable(Symbol& s) {

    }

    void Visitor::loadLocalArray(Symbol& s) {

    }
}
