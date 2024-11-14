//
// Created by tang on 11/8/24.
//

#ifndef SYMBOL1_HPP
#define SYMBOL1_HPP

#include <memory>
#include <vector>

namespace tang {
    class ObjectType;
    template <typename T>
    using s_ptr = std::shared_ptr<T>;

    /**
     * ObjectType --+-- BasicType --+-- IntType
     *              |               +-- CharType
     *              +-- ArrayType
     *              +-- FunctionType
     *              +-- VoidType
     */

    enum SymbolTypeID {
        INT_ST,
        CONST_INT_ST,
        CHAR_ST,
        CONST_CHAR_ST,
        INT_ARRAY_ST,
        CONST_INT_ARRAY_ST,
        CHAR_ARRAY_ST,
        CONST_CHAR_ARRAY_ST,
        VOID_FUNC_ST,
        INT_FUNC_ST,
        CHAR_FUNC_ST,
        VOID_ST,
    };

    class IntType : BasicType {
        bool isConst;
    public:
        int getSize() override { return 4; }
        int isConst() override { return isConst; }
    };

    class CharType : BasicType {
        bool isConst;
    public:
        int getSize() { return 1; }
        int isConst() { return isConst; }
    };

    class BasicType : ObjectType {
        bool isConst;
    public:
        BasicType(SymbolTypeID id, bool constant) : ObjectType
        virtual ~BasicType();
        bool isConst() { return isConst; }
    };

    class ArrayType : ObjectType {
        int _length;
        BasicType basicType;
    public:
        int getSize() override {
            return _length * basicType.getSize();
        }
        bool isConst() { return basicType.isConst(); }
    };

    class FunctionType : ObjectType {
        BasicType retType;
        std::vector<ObjectType> argType;
    public:
        int getSize() = 0;
        int argSize() { return argType.size(); }
    };

    class ObjectType {
        SymbolTypeID _id;
    public:
        ObjectType(SymbolTypeID id) : _id(id) {}
        virtual ~ObjectType();
        virtual int getSize() = 0;
        bool isArray() {
            return  id == INT_ARRAY_ST ||
                    id == CONST_INT_ARRAY_ST ||
                    id == CHAR_ARRAY_ST ||
                    id == CONST_CHAR_ARRAY_ST;
        }
        bool isFunc() {
            return  id == VOID_FUNC_ST ||
                    id == INT_FUNC_ST ||
                    id == CHAR_FUNC_ST;
        }
        bool isVoid() {
            return id == VOID_ST;
        }
        bool isConst() {
            return  id == CONST_INT_ST ||
                    id == CONST_CHAR_ST ||
                    id == CONST_INT_ARRAY_ST ||
                    id == CONST_CHAR_ARRAY_ST;
        }
        bool argMatch(ObjectType ty) {
            if (isArray() != ty.isArray()) {
                return false;
            }
            if (isArray() && ty.isArray()) {
                return true;
            }
            else {

            }
        }
    };

    class Symbol1 {

    };

}

#endif //SYMBOL1_HPP