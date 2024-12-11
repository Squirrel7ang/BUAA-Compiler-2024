//
// Created by tang on 11/21/24.
//

#ifndef GLOBALSTRING_HPP
#define GLOBALSTRING_HPP

#include "Common.hpp"
#include "GlobalValue.hpp"

namespace llvm {

    class GlobalString : public GlobalValue {
        std::string name;
        std::string escapeString;
        std::string asciiString;
        unsigned int size;
    public:
        explicit GlobalString(LLVMContextPtr& context, TypePtr ty, std::string& name, std::string& escString)
                : GlobalValue(context, ty, GLOBAL_STRING_T), name(name), escapeString(escString) {
            toAscii();
            size = escapeString.size() + 1;
        }
        explicit GlobalString(LLVMContextPtr& context, TypePtr ty, std::string&& name, std::string& escString)
                : GlobalValue(context, ty, GLOBAL_STRING_T), name(name), escapeString(escString) {
            toAscii();
            size = escapeString.size() + 1;
        }
        void print(std::ostream& out) override {
            out << '@' << name;
            out << " = " << "private ";
            out << "unnamed_addr ";
            out << "constant ";
            auto ty = std::static_pointer_cast<PointerType>(_type);
            ty->getBasicType()->print(out);
            out << " ";
            out << "c\"" << asciiString << "\"";
            out << std::endl;
        }
        void printRef(std::ostream &out) override {
            out << '@' << name;
        }
        std::string getName() { return name; }
    private:
        void toAscii() {
            for (int i = 0; i < escapeString.size(); i++) {
                char ch = escapeString.at(i);
                if (ch == '\n') {
                    asciiString += "\\0A";
                }
                else {
                    asciiString += ch;
                }
            }
            asciiString += "\\00";
        }
    };

}

#endif //GLOBALSTRING_HPP
