//
// Created by tang on 11/14/24.
//

#include "Visitor.hpp"

namespace tang {

    int Visitor::evaluate(u_ptr<Number>& node) {
        return node->intConst->val;
    }

    int Visitor::evaluate(u_ptr<Character>& node) {
        return node->charConst->ch;
    }

    int Visitor::evaluate(u_ptr<PrimaryExp>& node) {
        int ret;
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<Exp>>) {
                ret = evaluate(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<LVal>>) {
                ret = evaluate(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<Number>>) {
                ret = evaluate(arg);
            }
            else if constexpr (std::is_same_v<T, u_ptr<Character>>) {
                ret = evaluate(arg);
            }
            else
                assert(0);
        }, *(node->primaryExp));
        return ret;
    }

    int Visitor::evaluate(u_ptr<UnaryExp>& node) {
        auto& unaryExp = node->unaryExp;
        auto& unaryOps = node->unaryOps;
        int ret;
        if (std::holds_alternative<u_ptr<PrimaryExp>>(*unaryExp)) {
            auto& exp = std::get<u_ptr<PrimaryExp>>(*unaryExp);
            ret = evaluate(exp);
        }
        else assert(0); // FuncCall cannot be evaluated

        for (int i = unaryOps.size() - 1; i >= 0; i++) {
            u_ptr<UnaryOp>& op = unaryOps.at(i);
            if (op->isMinus) {
                ret = -ret;
            }
            else if (op->isExc) {
                ret = !ret;
            }
            // op->isPlus is ignored.
        }
        return ret;
    }

    int Visitor::evaluate(u_ptr<MulExp>& node) {
        int ret = evaluate(node->unaryExps[0]);
        int i = 1;
        for (auto op: node->ops) {
            if (op.getType() == TK_MULT)
                ret *= evaluate(node->unaryExps[i]);
            else if (op.getType() == TK_DIV)
                ret /= evaluate(node->unaryExps[i]);
            else assert(0);

            i++;
        }
        return ret;
    }

    int Visitor::evaluate(u_ptr<AddExp>& node) {
        int ret = evaluate(node->mulExps[0]);
        int i = 1;
        for (auto op: node->ops) {
            if (op.getType() == TK_MULT)
                ret += evaluate(node->mulExps[i]);
            else if (op.getType() == TK_DIV)
                ret -= evaluate(node->mulExps[i]);
            else assert(0);

            i++;
        }
        return ret;
    }

    int Visitor::evaluate(u_ptr<Exp>& node) {
        return evaluate(node->addExp);
    }

    int Visitor::evaluate(u_ptr<LVal>& node) {
        Symbol s;
        _symbolTable.findSymbolGlobal(s, node->ident->str);
        int idx = 0;
        if (s.getType()->isArray()) {
            idx = evaluate(node->exp);
        }

        int val = s.initValAt(idx);
        return val;
    }

    int Visitor::evaluate(u_ptr<ConstExp>& node) {
        return evaluate(node->addExp);
    }
}