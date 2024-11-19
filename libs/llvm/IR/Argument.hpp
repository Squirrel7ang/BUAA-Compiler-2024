//
// Created by tang on 11/18/24.
//

#ifndef ARGUMENT_HPP
#define ARGUMENT_HPP

#include <ostream>

#include "Common.hpp"
#include "Value.hpp"

namespace llvm{
    class Argument : public Value {
    public:
        explicit Argument(LLVMContextPtr context, TypePtr ty)
                : Value(context, ty, ARGUMENT_T) { }
        void print(std::ostream& out) {
            _type->print(out);
            out << " noundef ";

        }
    };
}

#endif //ARGUMENT_HPP
