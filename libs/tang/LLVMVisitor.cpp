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

    void Visitor::defineLocalVariable(const u_ptr<ConstDef>& node, Symbol& s) {
        auto context = _modulePtr->context();
        llvm::AllocaInstPtr aip = s.toAllocaInst(context);
        _curBlock->addInst(aip);

        // initialize;
        if (node->constInitVal != nullptr) {
            if (s.getType()->isArray()) {
                int i = 0;
                // if this is {} initializer
                if (node->constInitVal->stringConst == nullptr) {
                    for (auto& exp: node->constInitVal->constExps) {
                        auto vp = genConstExpIR(exp);
                        assignVariable(s, vp);
                        i++;
                    }
                }
                // if this is a stringConst;
                else {
                    auto& str = node->constInitVal->stringConst->str;
                    for (int i = 0; i < str->length(); i++) {
                        char ch = str->at(i);
                        auto vp = std::make_shared<llvm::ConstantData>(
                            context, context->I8_TY, ch);
                        assignVariable(s, vp);
                        i++;
                    }
                }
                // pad the rest to zero;
                for (;i < s.getType()->getLength(); i++) {
                    llvm::ConstantDataPtr zero = std::make_shared<llvm::ConstantData>(
                        context, s.getType()->toBasicLLVMType(context), 0);
                    assignVariable(s, zero);
                }
            }
            // else this is not an array
            else {
                auto vp = genConstExpIR(node->constInitVal->constExps.at(0));
                assignVariable(s, vp);
            }
        }
    }

    void Visitor::defineLocalVariable(const u_ptr<VarDef>& node, Symbol& s) {
        auto context = _modulePtr->context();
        llvm::AllocaInstPtr aip = s.toAllocaInst(context);
        _curBlock->addInst(aip);

        // initialize;
        if (node->initVal != nullptr) {
            if (s.getType()->isArray()) {
                int i = 0;
                // if this is {} initializer
                if (node->initVal->stringConst == nullptr) {
                    for (auto& exp: node->initVal->exps) {
                        auto vp = genExpIR(exp);
                        assignVariable(s, vp);
                        i++;
                    }
                }
                // if this is a stringConst;
                else {
                    auto& str = node->initVal->stringConst->str;
                    for (int i = 0; i < str->length(); i++) {
                        char ch = str->at(i);
                        auto vp = std::make_shared<llvm::ConstantData>(
                            context, context->I8_TY, ch);
                        assignVariable(s, vp);
                        i++;
                    }
                }
                // pad the rest to zero;
                for (;i < s.getType()->getLength(); i++) {
                    llvm::ConstantDataPtr zero = std::make_shared<llvm::ConstantData>(
                        context, s.getType()->toBasicLLVMType(context), 0);
                    assignVariable(s, zero);
                }
            }
            // else this is not an array
            else {
                auto vp = genExpIR(node->initVal->exps.at(0));
                assignVariable(s, vp);
            }
        }
    }

    llvm::ValuePtr Visitor::genConstExpIR(const u_ptr<ConstExp>& node) {
        return genAddExpIR(node->addExp);
    }

    llvm::ValuePtr Visitor::genExpIR(const u_ptr<Exp>& node) {
        return genAddExpIR(node->addExp);
    }

    llvm::ValuePtr Visitor::genAddExpIR(const u_ptr<AddExp>& node) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst0 = genMulExpIR(node->mulExps.at(0));
        llvm::ValuePtr inst1;

        for (int i = 0; i < node->ops.size(); i++) {
            inst1 = genMulExpIR(node->mulExps.at(i+1));
            llvm::BinaryOperatorPtr bip;
            if (node->ops.at(i).getType() == TK_PLUS) {
                bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_ADD);
            }
            else if (node->ops.at(i).getType() == TK_MINU) {
                bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_SUB);
            }
            else
                assert(0);

            _curBlock->addInst(bip);
            inst0 = bip;
        }
        return inst0;
    }

    llvm::ValuePtr Visitor::genMulExpIR(const u_ptr<MulExp>& node) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst0 = genUnaryExpIR(node->unaryExps.at(0));
        llvm::ValuePtr inst1;
        for (int i = 0; i < node->ops.size(); i++) {
            inst1 = genUnaryExpIR(node->unaryExps.at(i+1));
            llvm::BinaryOperatorPtr bip;
            if (node->ops.at(i).getType() == TK_MULT) {
                bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_MUL);
            }
            else if (node->ops.at(i).getType() == TK_DIV) {
                bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_DIV);
            }
            else if (node->ops.at(i).getType() == TK_MOD) {
                bip = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, inst0, inst1, llvm::BOID_REM);
            }
            else
                assert(0);

            _curBlock->addInst(bip);
            inst0 = bip;
        }
        return inst0;
    }

    llvm::ValuePtr Visitor::genUnaryExpIR(const u_ptr<UnaryExp>& node) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst;

        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<FuncCall>>) {
                inst = genFuncCallIR(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<PrimaryExp>>) {
                inst = genPrimaryExp(arg);
            }
            else assert(0);
        }, *(node->unaryExp));

        llvm::UnaryOperatorPtr uop;
        for (auto& op: node->unaryOps) {
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

        if (uop != nullptr) {
            return uop;
        }
        else {
            return inst;
        }
    }

    llvm::ValuePtr Visitor::genPrimaryExp(const u_ptr<PrimaryExp>& node) {
        llvm::ValuePtr inst;
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<Exp>>) {
                inst = genExpIR(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<Number>>) {
                inst = genNumberIR(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<Character>>) {
                inst = genCharacterIR(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<LVal>>) {
                inst = genLValIR(arg);
            }
            else assert(0);
        }, *(node->primaryExp));
        return inst;
    }

    llvm::CallInstPtr Visitor::genFuncCallIR(const u_ptr<FuncCall>& node) {
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
            for (auto& exp: node->funcRParams->exps) {
                auto inst = genExpIR(exp);
                rargs.push_back(inst);
            }
            cip = std::make_shared<llvm::CallInst>(context, _llvm_type, rargs);
        }

        _curBlock->addInst(cip);

        return cip;

    }

    llvm::ConstantDataPtr Visitor::genNumberIR(const u_ptr<Number>& node) {
        auto context = _modulePtr->context();
        llvm::ConstantDataPtr cdp = std::make_shared<llvm::ConstantData>(
            context, context->I32_TY, node->intConst->val);
        return cdp;
    }

    llvm::ConstantDataPtr Visitor::genCharacterIR(const u_ptr<Character>& node) {
        auto context = _modulePtr->context();
        llvm::ConstantDataPtr cdp = std::make_shared<llvm::ConstantData>(
            context, context->I8_TY, node->charConst->ch);
        return cdp;
    }

    llvm::LoadInstPtr Visitor::genLValIR(const u_ptr<LVal>& node) {
        auto context = _modulePtr->context();
        Symbol s;
        _symbolTable.findSymbolGlobal(s, node->ident->str);

        llvm::LoadInstPtr lip;
        if (s.getType()->isArray()) {
            llvm::GetElePtrInstPtr gepi;
            auto offset = genExpIR(node->exp);
            if (s.getType()->toRawType() == INT_ARRAY_ST) {
                auto sty = std::dynamic_pointer_cast<IntArrayType>(s.getType());
                gepi = std::make_shared<llvm::GetElePtrInst>(
                    context, sty->toLLVMType(context), sty->toBasicLLVMType(context),
                    s.getLLVMValue(), offset);
                lip = std::make_shared<llvm::LoadInst>(
                    context, sty->toBasicLLVMType(context), gepi);
            }
            else if (s.getType()->toRawType() == CHAR_ARRAY_ST) {
                auto sty = std::dynamic_pointer_cast<CharArrayType>(s.getType());
                gepi = std::make_shared<llvm::GetElePtrInst>(
                    context, sty->toLLVMType(context), context->I8_TY, s.getLLVMValue(), offset);
                lip = std::make_shared<llvm::LoadInst>(
                    context, context->I8_TY, gepi);
            }
            _curBlock->addInst(gepi);
        }
        else {
            lip = std::make_shared<llvm::LoadInst>(
                context, s.getType()->toLLVMType(context), s.getLLVMValue());
        }
        _curBlock->addInst(lip);

        return lip;
    }

    void Visitor::assignVariable(Symbol& s, llvm::ValuePtr value) {
        auto context = _modulePtr->context();
        auto ty = s.getLLVMValue();
        auto ptr = s.getLLVMValue();
        auto sip = std::make_shared<llvm::StoreInst>(
            context, s.getType()->toBasicLLVMType(context), value, ptr);
        _curBlock->addInst(sip);
    }

    void Visitor::returnValue(llvm::TypePtr ty, llvm::ValuePtr value) {
        auto context = _modulePtr->context();
        llvm::ReturnInstPtr rip = std::make_shared<llvm::ReturnInst>(
            context, ty, value);
    }

    void Visitor::returnVoid() {
        auto context = _modulePtr->context();
        llvm::ReturnInstPtr rip = std::make_shared<llvm::ReturnInst>( context);

    }
}
