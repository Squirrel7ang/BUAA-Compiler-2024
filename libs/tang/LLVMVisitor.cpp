//
// Created by tang on 11/14/24.
//

#include <valarray>

#include "Visitor.hpp"
#include "IR/GlobalString.hpp"

namespace tang {
    llvm::ValuePtr Visitor::convert(llvm::ValuePtr value, llvm::TypePtr target) {
        auto context = _modulePtr->context();
        if (!target->isInteger()) { assert(0); }
        if (!value->getType()->isInteger()) {
            // assert(0);
            // this could be a pointer type, since array could be involved in exp.
            return value;
        }

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

    // TODO: initialize the global value
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
                llvm::ConstantDataPtr offset;
                if (node->constInitVal->stringConst == nullptr) {
                    for (auto& exp: node->constInitVal->constExps) {
                        auto vp = genConstExpIR(exp, s.getType()->toBasicLLVMType(context));
                        offset = std::make_shared<llvm::ConstantData>(context, context->I32_TY, i);
                        assignLVal(s, offset, vp);
                        i++;
                    }
                }
                // if this is a stringConst;
                else {
                    auto& str = node->constInitVal->stringConst->_str;
                    for (; i < str.length(); i++) {
                        char ch = str.at(i);
                        auto vp = std::make_shared<llvm::ConstantData>(
                            context, context->I8_TY, ch);
                        offset = std::make_shared<llvm::ConstantData>(context, context->I32_TY, i);
                        assignLVal(s, offset, vp);
                    }
                }
                // pad the rest to zero;
                for (;i < s.getType()->getLength(); i++) {
                    llvm::ConstantDataPtr zero = std::make_shared<llvm::ConstantData>(
                        context, s.getType()->toBasicLLVMType(context), 0);
                    offset = std::make_shared<llvm::ConstantData>(context, context->I32_TY, i);
                    assignLVal(s, offset, zero);
                }
            }
            // else this is not an array, but a normal variable;
            else {
                auto vp = genConstExpIR(node->constInitVal->constExps.at(0), s.getType()->toLLVMType(context));
                assignLVal(s, vp);
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
                llvm::ConstantDataPtr offset;
                if (node->initVal->stringConst == nullptr) {
                    for (auto& exp: node->initVal->exps) {
                        auto vp = genExpIR(exp, s.getType()->toBasicLLVMType(context));
                        offset = std::make_shared<llvm::ConstantData>(context, context->I32_TY, i);
                        assignLVal(s, offset, vp);
                        i++;
                    }
                }
                // if this is a stringConst;
                else {
                    auto& str = node->initVal->stringConst->_str;
                    for (; i < str.length(); i++) {
                        char ch = str.at(i);
                        auto vp = std::make_shared<llvm::ConstantData>(
                            context, context->I8_TY, ch);
                        offset = std::make_shared<llvm::ConstantData>(context, context->I32_TY, i);
                        assignLVal(s, offset, vp);
                    }
                }
                // pad the rest to zero;
                for (;i < s.getType()->getLength(); i++) {
                    llvm::ConstantDataPtr zero = std::make_shared<llvm::ConstantData>(
                        context, s.getType()->toBasicLLVMType(context), 0);
                    offset = std::make_shared<llvm::ConstantData>(context, context->I32_TY, i);
                    assignLVal(s, offset, zero);
                }
            }
            // else this is not an array
            else {
                auto vp = genExpIR(node->initVal->exps.at(0), s.getType()->toLLVMType(context));
                assignLVal(s, vp);
            }
        }
    }

    llvm::ValuePtr Visitor::genConstExpIR(const u_ptr<ConstExp>& node, const llvm::TypePtr& expectType) {
        return genAddExpIR(node->addExp, expectType);
    }

    llvm::ValuePtr Visitor::genExpIR(const u_ptr<Exp>& node, const llvm::TypePtr& expectType) {
        auto&& ret = genAddExpIR(node->addExp, _modulePtr->context()->I32_TY);
        return convert(ret, expectType);
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

        llvm::CompareInstPtr cip;
        llvm::BinaryOperatorPtr bop;
        for (auto& op: node->unaryOps) {
            llvm::ConstantDataPtr zero = std::make_shared<llvm::ConstantData>(context, context->I32_TY, 0);
            if (op->isMinus) {
                bop = std::make_shared<llvm::BinaryOperator>(
                    context, context->I32_TY, zero, inst, llvm::BOID_SUB);
                _curBlock->addInst(bop);
            }
            else if (op->isExc) {
                cip = std::make_shared<llvm::CompareInst>(
                    context, zero, inst, llvm::CIID_EQ);
                _curBlock->addInst(cip);
            }
        }

        if (cip != nullptr) {
            return convert(cip, expectType);
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
        if (!s.getType()->isArray()) {
            lip = std::make_shared<llvm::LoadInst>(context, s.getType()->toLLVMType(context), s.getLLVMValue());
        }
        else if (node->exp == nullptr) {
            // we are trying to pass an array into function parameter.
            llvm::GetElePtrInstPtr gepi;
            auto offset = std::make_shared<llvm::ConstantData>(context, context->I32_TY, 0);
            if (s.getType()->toRawType() == INT_ARRAY_ST ||
                s.getType()->toRawType() == CONST_INT_ARRAY_ST) {
                auto sty = std::dynamic_pointer_cast<IntArrayType>(s.getType());
                if (sty->isArgument()) {
                    auto preload = std::make_shared<llvm::LoadInst>(
                        context, context->I32_PTR_TY, s.getLLVMValue()); // preload is a pointer to i32
                    _curBlock->addInst(preload);
                    gepi = std::make_shared<llvm::GetElePtrInst>(
                        context, context->I32_PTR_TY, preload, offset);
                }
                else {
                    gepi = std::make_shared<llvm::GetElePtrInst>(
                        context, context->I32_PTR_TY, s.getLLVMValue(), offset);
                }
            }
            else if (s.getType()->toRawType() == CHAR_ARRAY_ST ||
                     s.getType()->toRawType() == CONST_CHAR_ARRAY_ST) {
                auto sty = std::dynamic_pointer_cast<CharArrayType>(s.getType());
                if (sty->isArgument()) {
                    // this is an argument, thus a pointer type
                    auto preload = std::make_shared<llvm::LoadInst>(
                        context, context->I8_PTR_TY, s.getLLVMValue()); // preload is a pointer to i32
                    _curBlock->addInst(preload);
                    gepi = std::make_shared<llvm::GetElePtrInst>(
                        context, context->I8_PTR_TY, preload, offset);
                }
                else {
                    gepi = std::make_shared<llvm::GetElePtrInst>(
                        context, context->I8_PTR_TY, s.getLLVMValue(), offset);
                }
            }
            _curBlock->addInst(gepi);
            return gepi; // the only place where it is not convert to expectType.
        }
        else {
            llvm::GetElePtrInstPtr gepi;
            auto offset = genExpIR(node->exp, expectType);
            if (s.getType()->toRawType() == INT_ARRAY_ST ||
                s.getType()->toRawType() == CONST_INT_ARRAY_ST) {
                auto sty = std::dynamic_pointer_cast<IntArrayType>(s.getType());
                if (sty->isArgument()) {
                    // this is an argument, thus a pointer type
                    auto preload = std::make_shared<llvm::LoadInst>(
                        context, context->I32_PTR_TY, s.getLLVMValue()); // preload is a pointer to i32
                    _curBlock->addInst(preload);
                    gepi = std::make_shared<llvm::GetElePtrInst>(
                        context, context->I32_PTR_TY, preload, offset);
                }
                else {
                    gepi = std::make_shared<llvm::GetElePtrInst>(
                        context, context->I32_PTR_TY, s.getLLVMValue(), offset);
                }
                lip = std::make_shared<llvm::LoadInst>(context, context->I32_TY, gepi);
            }
            else if (s.getType()->toRawType() == CHAR_ARRAY_ST ||
                     s.getType()->toRawType() == CONST_CHAR_ARRAY_ST) {
                auto sty = std::dynamic_pointer_cast<CharArrayType>(s.getType());
                if (sty->isArgument()) {
                    // this is an argument, thus a pointer type
                    auto preload = std::make_shared<llvm::LoadInst>(
                        context, context->I8_PTR_TY, s.getLLVMValue()); // preload is a pointer to i32
                    _curBlock->addInst(preload);
                    gepi = std::make_shared<llvm::GetElePtrInst>(
                        context, context->I8_PTR_TY, preload, offset);
                }
                else {
                    gepi = std::make_shared<llvm::GetElePtrInst>(
                        context, context->I8_PTR_TY, s.getLLVMValue(), offset);
                }
                lip = std::make_shared<llvm::LoadInst>(context, context->I8_TY, gepi);
            }
            _curBlock->addInst(gepi);
        }
        _curBlock->addInst(lip);

        return convert(lip, expectType);
    }

    void Visitor::genGetcharStmtIR(const u_ptr<GetcharStmt>& node) {
        // llvm
        auto&& context = _modulePtr->context();

        Symbol s;
        _symbolTable.findSymbolGlobal(s, node->lVal->ident->str);

        auto inst = std::make_shared<llvm::GetcharInst>(context);
        _curBlock->addInst(inst);
        auto&& val = convert(inst, s.getType()->toBasicLLVMType(context));

        assignLVal(s, val);
    }

    void Visitor::genGetintStmtIR(const u_ptr<GetintStmt>& node) {
        // llvm
        auto&& context = _modulePtr->context();

        Symbol s;
        _symbolTable.findSymbolGlobal(s, node->lVal->ident->str);

        auto inst = std::make_shared<llvm::GetintInst>(context);
        _curBlock->addInst(inst);
        auto&& val = convert(inst, s.getType()->toBasicLLVMType(context));

        assignLVal(s, val);
    }

    void Visitor::genPrintfStmtIR(const u_ptr<PrintfStmt>& node) {
        // llvm
        auto&& context = _modulePtr->context();
        auto& rawStr = node->stringConst->_str;
        char ch0, ch1;
        int expNum = 0;
        std::string tmpStr;

        vector<llvm::ValuePtr> args;
        for (auto& exp: node->exps) {
            auto&& sty = node->symbolTypeAt(expNum);
            expNum++;
            // args.push_back(genExpIR(exp, sty->toLLVMType(context)));
            // putch  is also of I32 type
            args.push_back(genExpIR(exp, context->I32_TY));
        }
        expNum = 0;

        ch0 = rawStr.at(0);
        tmpStr += ch0;
        for (int i = 1; i < rawStr.size(); i++) {
            ch1 = rawStr.at(i);
            tmpStr += ch1;
            if (ch0 == '%' && (ch1 == 'd' || ch1 == 'c')) {
                auto size = tmpStr.size();
                tmpStr = tmpStr.substr(0, size - 2);
                // handle tmpStr
                if (!tmpStr.empty()) {
                    auto llvmType = std::make_shared<llvm::ArrayType>(context->I8_TY, tmpStr.size() + 1);
                    auto ty = std::make_shared<llvm::PointerType>(llvmType);
                    auto globalStr = std::make_shared<llvm::GlobalString>(
                        context, ty, context->getStringName(), tmpStr);
                    _modulePtr->addString(globalStr);
                    genPutstrIR(globalStr);
                }
                // handle %c or %d
                if (ch1 == 'c') {
                    auto&& value = args.at(expNum++);
                    genPutchIR(value);
                }
                else {
                    auto&& value = args.at(expNum++);
                    genPutintIR(value);
                }
                // reset tmpStr
                tmpStr = "";
            }
            ch0 = ch1;
        }

        if (!tmpStr.empty()) {
            auto&& llvmType = std::make_shared<llvm::ArrayType>(context->I8_TY, tmpStr.size() + 1);
            auto ty = std::make_shared<llvm::PointerType>(llvmType);
            auto&& globalStr = std::make_shared<llvm::GlobalString>(
                context, ty, context->getStringName(), tmpStr);
            _modulePtr->addString(globalStr);
            genPutstrIR(globalStr);
        }
    }

    llvm::ValuePtr Visitor::genPutchIR(llvm::ValuePtr value) {
        auto&& context = _modulePtr->context();
        auto putch = std::make_shared<llvm::PutInst>(context, value, llvm::PIID_CH);
        _curBlock->addInst(putch);
        return putch;
    }

    llvm::ValuePtr Visitor::genPutintIR(llvm::ValuePtr value) {
        auto&& context = _modulePtr->context();
        auto putint = std::make_shared<llvm::PutInst>(context, value, llvm::PIID_INT);
        _curBlock->addInst(putint);
        return putint;
    }

    llvm::ValuePtr Visitor::genPutstrIR(llvm::GlobalStringPtr value) {
        auto&& context = _modulePtr->context();
        auto zero = std::make_shared<llvm::ConstantData>(context, context->I32_TY, 0);
        auto geip = std::make_shared<llvm::GetElePtrInst>(
            context, context->I8_PTR_TY, value, zero);
        _curBlock->addInst(geip);
        auto putstr = std::make_shared<llvm::PutInst>(context, geip, llvm::PIID_STR);
        _curBlock->addInst(putstr);
        return putstr;
    }

    void Visitor::genCondIR(const u_ptr<Cond>& node, const llvm::BasicBlockPtr &ifBlock,
                            const llvm::BasicBlockPtr &elseBlock) {
        genLOrExpIR(node->lOrExp, ifBlock, elseBlock);
    }

    void Visitor::genLOrExpIR(const u_ptr<LOrExp>& node,
                              const llvm::BasicBlockPtr &ifBlock, const llvm::BasicBlockPtr &elseBlock) {
        auto&& context = _modulePtr->context();

        int i;
        for (i = 0; i < node->lAndExps.size() - 1; i++) {
            llvm::BasicBlockPtr newBlock = std::make_shared<llvm::BasicBlock>(context);
            genLAndExpIR(node->lAndExps.at(i), ifBlock, newBlock);
            _curBlock = newBlock;
            _curFunction->addBasicBlock(newBlock);
        }
        genLAndExpIR(node->lAndExps.at(i), ifBlock, elseBlock);
        _curBlock = ifBlock;
        _curFunction->addBasicBlock(ifBlock);
    }

    void Visitor::genLAndExpIR(const u_ptr<LAndExp>& node,
                               const llvm::BasicBlockPtr& ifBlock, const llvm::BasicBlockPtr& elseBlock) {
        auto&& context = _modulePtr->context();
        llvm::ValuePtr inst;

        int i;
        for (i = 0; i < node->eqExps.size() - 1; i++) {
            llvm::BasicBlockPtr newBlock = std::make_shared<llvm::BasicBlock>(context);
            inst = genEqExpIR(node->eqExps.at(i), context->I1_TY);
            auto brInst = std::make_shared<llvm::BranchInst>(
                context, inst, newBlock, elseBlock);
            _curBlock->addInst(brInst);
            _curBlock = newBlock;
            _curFunction->addBasicBlock(newBlock);
        }
        inst = genEqExpIR(node->eqExps.at(i), context->I1_TY);
        auto brInst = std::make_shared<llvm::BranchInst>(
            context, inst, ifBlock, elseBlock);
        _curBlock->addInst(brInst);
    }

    llvm::ValuePtr Visitor::genEqExpIR(const u_ptr<EqExp>& node, const llvm::TypePtr& expectType) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst0 = genRelExpIR(node->relExps.at(0), context->I32_TY);
        llvm::ValuePtr inst1;

        for (int i = 0; i < node->ops.size(); i++) {
            inst1 = genRelExpIR(node->relExps.at(i+1), context->I32_TY);
            llvm::CompareInstPtr cip;
            auto&& opType = node->ops.at(i).getType();
            if (opType == TK_EQL) {
                cip = std::make_shared<llvm::CompareInst>(
                    context, inst0, inst1, llvm::CIID_EQ);
            }
            else if (opType == TK_NEQ) {
                cip = std::make_shared<llvm::CompareInst>(
                    context, inst0, inst1, llvm::CIID_NE);
            }
            else
                assert(0);

            _curBlock->addInst(cip);
            inst0 = cip;
        }

        if (node->ops.empty()) {
            auto zero = std::make_shared<llvm::ConstantData>(context, context->I32_TY, 0);
            auto cip = std::make_shared<llvm::CompareInst>(
                context, inst0, zero, llvm::CIID_NE);
            _curBlock->addInst(cip);
            return cip;
        }

        return convert(inst0, expectType);
    }

    llvm::ValuePtr Visitor::genRelExpIR(const u_ptr<RelExp>& node, const llvm::TypePtr& expectType) {
        auto context = _modulePtr->context();
        llvm::ValuePtr inst0 = genAddExpIR(node->addExps.at(0), context->I32_TY);
        llvm::ValuePtr inst1;

        for (int i = 0; i < node->ops.size(); i++) {
            inst1 = genAddExpIR(node->addExps.at(i+1), context->I32_TY);
            llvm::CompareInstPtr cip;
            auto&& opType = node->ops.at(i).getType();
            if (opType == TK_LEQ) {
                cip = std::make_shared<llvm::CompareInst>(
                    context, inst0, inst1, llvm::CIID_SLE);
            }
            else if (opType == TK_LSS) {
                cip = std::make_shared<llvm::CompareInst>(
                    context, inst0, inst1, llvm::CIID_SLT);
            }
            else if (opType == TK_GEQ) {
                cip = std::make_shared<llvm::CompareInst>(
                    context, inst0, inst1, llvm::CIID_SGE);
            }
            else if (opType == TK_GRE) {
                cip = std::make_shared<llvm::CompareInst>(
                    context, inst0, inst1, llvm::CIID_SGT);
            }
            else
                assert(0);

            _curBlock->addInst(cip);
            inst0 = convert(cip, context->I32_TY);
        }
        return convert(inst0, expectType);
    }

    void Visitor::assignLVal(Symbol& s, llvm::ValuePtr value) {
        auto&& context = _modulePtr->context();

        llvm::StoreInstPtr sip;
        auto ty = s.getLLVMValue();
        auto ptr = s.getLLVMValue();
        sip = std::make_shared<llvm::StoreInst>(
            context, value, ptr);
        _curBlock->addInst(sip);
    }

    void Visitor::assignLVal(Symbol& s, llvm::ValuePtr offset, llvm::ValuePtr value) {
        auto&& context = _modulePtr->context();
        if (offset == nullptr) {
            assignLVal(s, value);
            return;
        }

        llvm::StoreInstPtr sip;

        llvm::GetElePtrInstPtr gepi;
        if (s.getType()->toRawType() == INT_ARRAY_ST ||
            s.getType()->toRawType() == CONST_INT_ARRAY_ST) {
            auto&& sty = std::static_pointer_cast<IntArrayType>(s.getType());
            if (sty->isArgument()) {
                // load first, then getelementptr
                auto preload = std::make_shared<llvm::LoadInst>(
                    context, context->I32_PTR_TY, s.getLLVMValue());
                _curBlock->addInst(preload);
                gepi = std::make_shared<llvm::GetElePtrInst>(
                    context, context->I32_PTR_TY, preload, offset);
            }
            else {
                // getElementPtr
                gepi = std::make_shared<llvm::GetElePtrInst>(
                    context, context->I32_PTR_TY, s.getLLVMValue(), offset);
            }
        }
        else if (s.getType()->toRawType() == CHAR_ARRAY_ST ||
                 s.getType()->toRawType() == CONST_CHAR_ARRAY_ST) {
            auto&& sty = std::static_pointer_cast<CharArrayType>(s.getType());
            if (sty->isArgument()) {
                // load first, then getelementptr
                auto preload = std::make_shared<llvm::LoadInst>(
                    context, context->I32_PTR_TY, s.getLLVMValue());
                _curBlock->addInst(preload);
                gepi = std::make_shared<llvm::GetElePtrInst>(
                    context, context->I32_PTR_TY, preload, offset);
            }
            else {
                // getElementPtr
                gepi = std::make_shared<llvm::GetElePtrInst>(
                    context, context->I32_PTR_TY, s.getLLVMValue(), offset);
            }
        }
        _curBlock->addInst(gepi);
        sip = std::make_shared<llvm::StoreInst>(context, value, gepi);
        _curBlock->addInst(sip);
    }

    void Visitor::assignLVal(u_ptr<LVal>& lVal, llvm::ValuePtr value) {
        auto&& context = _modulePtr->context();
        Symbol s;
        _symbolTable.findSymbolGlobal(s, lVal->ident->str);

        if (!s.getType()->isArray()) {
            assignLVal(s, value);
        }
        else {
            auto offset = genExpIR(lVal->exp, context->I32_TY);
            assignLVal(s, offset, value);
        }
    }

    void Visitor::returnValue(llvm::TypePtr ty, llvm::ValuePtr value) {
        auto context = _modulePtr->context();
        llvm::ReturnInstPtr rip = std::make_shared<llvm::ReturnInst>(
            context, value);
        _curBlock->addInst(rip);

        // add new block after return statement
        auto newBlock = std::make_shared<llvm::BasicBlock>(context);
        _curFunction->addBasicBlock(newBlock);
        _curBlock = newBlock;
    }

    void Visitor::returnVoid() {
        auto context = _modulePtr->context();
        llvm::ReturnInstPtr rip = std::make_shared<llvm::ReturnInst>( context);
        _curBlock->addInst(rip);

        // add new block after return statement
        auto newBlock = std::make_shared<llvm::BasicBlock>(context);
        _curFunction->addBasicBlock(newBlock);
        _curBlock = newBlock;
    }
}
