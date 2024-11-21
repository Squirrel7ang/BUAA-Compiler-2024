//
// Created by tang on 11/14/24.
//

#include <valarray>

#include "Visitor.hpp"

namespace tang {
    llvm::ValuePtr Visitor::convert(llvm::ValuePtr value, llvm::TypePtr target) {
        auto context = _modulePtr->context();
        if (!target->isInteger()) { assert(0); }
        if (!value->getType()->isInteger()) { assert(0); }

        auto _origin = std::static_pointer_cast<llvm::IntegerType>(value->getType());
        auto _target = std::static_pointer_cast<llvm::IntegerType>(target);

        int origin_bits = _origin->getBits();
        int target_bits = _target->getBits();

        if (origin_bits < target_bits) {
            // zext
            auto zext  = std::make_shared<llvm::UnaryOperator>(context, _target, value, llvm::UOID_ZEXT);
            _curBlock->addInst(zext);
            return zext;
        }
        else if (origin_bits > target_bits) {
            // trunc
            auto trunc = std::make_shared<llvm::UnaryOperator>(context, _target, value, llvm::UOID_TRUNC);
            _curBlock->addInst(trunc);
            return trunc;
        }
        // else return the original value
        return value;
    }

    void Visitor::defineGlobalVariable(Symbol& s) {
        auto&& context = _modulePtr->context();
        llvm::GlobalVariablePtr gv = s.toGlobalVariable(context);
        _modulePtr->addGlobalVariable(gv);
    }

    void Visitor::defineLocalVariable(const u_ptr<ConstDef>& node, Symbol& s) {
        auto&& context = _modulePtr->context();
        llvm::AllocaInstPtr aip = s.toAllocaInst(context);
        _curBlock->addInst(aip);

        // initialize;
        if (node->constInitVal != nullptr) {
            if (s.getType()->isArray()) {
                int i = 0;
                // if this is {} initializer
                if (node->constInitVal->stringConst == nullptr) {
                    for (auto& exp: node->constInitVal->constExps) {
                        auto vp = genConstExpIR(exp, s.getType()->toBasicLLVMType(context));
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
                auto vp = genConstExpIR(node->constInitVal->constExps.at(0), s.getType()->toLLVMType(context));
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
                        auto vp = genExpIR(exp, s.getType()->toBasicLLVMType(context));
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
                auto vp = genExpIR(node->initVal->exps.at(0), s.getType()->toLLVMType(context));
                assignVariable(s, vp);
            }
        }
    }

    llvm::ValuePtr Visitor::genConstExpIR(const u_ptr<ConstExp>& node, const llvm::TypePtr& expectType) {
        return genAddExpIR(node->addExp, expectType);
    }

    llvm::ValuePtr Visitor::genExpIR(const u_ptr<Exp>& node, const llvm::TypePtr& expectType) {
        return genAddExpIR(node->addExp, expectType);
    }

    llvm::ValuePtr Visitor::genAddExpIR(const u_ptr<AddExp>& node, const llvm::TypePtr& expectType) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst0 = genMulExpIR(node->mulExps.at(0), expectType);
        llvm::ValuePtr inst1;

        for (int i = 0; i < node->ops.size(); i++) {
            inst1 = genMulExpIR(node->mulExps.at(i+1), expectType);
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
        // TODO: match expectType
        return convert(inst0, expectType);
    }

    llvm::ValuePtr Visitor::genMulExpIR(const u_ptr<MulExp>& node, const llvm::TypePtr& expectType) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst0 = genUnaryExpIR(node->unaryExps.at(0), expectType);
        llvm::ValuePtr inst1;
        for (int i = 0; i < node->ops.size(); i++) {
            inst1 = genUnaryExpIR(node->unaryExps.at(i+1), expectType);
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
        return convert(inst0, expectType);
    }

    llvm::ValuePtr Visitor::genUnaryExpIR(const u_ptr<UnaryExp>& node, const llvm::TypePtr& expectType) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst;

        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<FuncCall>>) {
                inst = genFuncCallIR(arg, expectType);
            }
            else if constexpr (std::is_same_v<T, u_ptr<PrimaryExp>>) {
                inst = genPrimaryExp(arg, expectType);
            }
            else assert(0);
        }, *(node->unaryExp));

        llvm::UnaryOperatorPtr uop; //  TODO: there are no unary instructions
        llvm::BinaryOperatorPtr bop;
        for (auto& op: node->unaryOps) {
            if (op->isMinus) {
                llvm::ConstantDataPtr cd = std::make_shared<llvm::ConstantData>(context, context->I32_TY, 0);
                bop = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, cd, inst, llvm::BOID_SUB);
                _curBlock->addInst(bop);
            }
            else if (op->isExc) {
                // TODO
            }
        }

        if (uop != nullptr) {
            return convert(uop, expectType);
        }
        else if (bop != nullptr) {
            return convert(bop, expectType);
        }
        else {
            return convert(inst, expectType);
        }
    }

    llvm::ValuePtr Visitor::genPrimaryExp(const u_ptr<PrimaryExp>& node, const llvm::TypePtr& expectType) {
        llvm::ValuePtr inst;
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<Exp>>) {
                inst = genExpIR(arg, expectType);
            }
            else if constexpr (std::is_same_v<T, u_ptr<Number>>) {
                inst = genNumberIR(arg, expectType);
            }
            else if constexpr (std::is_same_v<T, u_ptr<Character>>) {
                inst = genCharacterIR(arg, expectType);
            }
            else if constexpr (std::is_same_v<T, u_ptr<LVal>>) {
                inst = genLValIR(arg, expectType);
            }
            else assert(0);
        }, *(node->primaryExp));
        return convert(inst, expectType);
    }

    llvm::ValuePtr Visitor::genFuncCallIR(const u_ptr<FuncCall>& node, const llvm::TypePtr& expectType) {
        auto context = _modulePtr->context();
        llvm::CallInstPtr cip;
        Symbol s;
        _symbolTable.findSymbolGlobal(s, node->ident->str);
        assert(s.getType()->isFunc());
        auto _func_type= std::dynamic_pointer_cast<FuncSymbolType>(s.getType());
        auto _llvm_type = _func_type->_returnType->toLLVMType(context);

        if (node->funcRParams == nullptr) {
            cip = std::make_shared<llvm::CallInst>(context, _llvm_type, s.getLLVMValue());
        }
        else {
            vector<llvm::ValuePtr> rargs;
            for (auto& exp: node->funcRParams->exps) {
                auto inst = genExpIR(exp, expectType);
                rargs.push_back(inst);
            }
            cip = std::make_shared<llvm::CallInst>(context, _llvm_type, s.getLLVMValue(), rargs);
        }

        _curBlock->addInst(cip);

        // TODO: match expectType
        return convert(cip, expectType);

    }

    llvm::ValuePtr Visitor::genNumberIR(const u_ptr<Number>& node, const llvm::TypePtr& expectType) {
        auto context = _modulePtr->context();
        llvm::ConstantDataPtr cdp = std::make_shared<llvm::ConstantData>(
            context, context->I32_TY, node->intConst->val);
        return convert(cdp, expectType);
    }

    llvm::ValuePtr Visitor::genCharacterIR(const u_ptr<Character>& node, const llvm::TypePtr& expectType) {
        auto context = _modulePtr->context();
        llvm::ConstantDataPtr cdp = std::make_shared<llvm::ConstantData>(
            context, context->I8_TY, node->charConst->ch);
        return convert(cdp, expectType);
    }

    llvm::ValuePtr Visitor::genLValIR(const u_ptr<LVal>& node, const llvm::TypePtr& expectType) {
        auto context = _modulePtr->context();
        Symbol s;
        _symbolTable.findSymbolGlobal(s, node->ident->str);

        llvm::LoadInstPtr lip;
        if (s.getType()->isArray()) {
            llvm::GetElePtrInstPtr gepi;
            auto offset = genExpIR(node->exp, expectType);
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

        return convert(lip, expectType);
    }

    void Visitor::assignVariable(Symbol& s, llvm::ValuePtr value) {
        auto context = _modulePtr->context();
        auto ty = s.getLLVMValue();
        auto ptr = s.getLLVMValue();
        auto sip = std::make_shared<llvm::StoreInst>(
            context, value, ptr);
        _curBlock->addInst(sip);
    }

    void Visitor::returnValue(llvm::TypePtr ty, llvm::ValuePtr value) {
        auto context = _modulePtr->context();
        llvm::ReturnInstPtr rip = std::make_shared<llvm::ReturnInst>(
            context, ty, value);
        _curBlock->addInst(rip);
    }

    void Visitor::returnVoid() {
        auto context = _modulePtr->context();
        llvm::ReturnInstPtr rip = std::make_shared<llvm::ReturnInst>( context);
        _curBlock->addInst(rip);
    }
}
