//
// Created by tang on 11/7/24.
//

#ifndef TYPE_HPP
#define TYPE_HPP

#include <memory>

#include "LLVMContext.hpp"

namespace llvm {
    /**
     * Different from ValueType, which refers to the
     * type of Value Object.
     *
     * Type refers to the Type of the data, such as
     * int8, int32, functionType, voidType,PointerType
     */
    class Type {
    public:
        enum TypeID {
            VOID_TYPE_ID,
            LABEL_TYPE_ID,
            INTEGER_TYPE_ID,
            ARRAY_TYPE_ID,
            FUNCTION_TYPE_ID,
            POINTER_TYPE_ID,
        };
        // virtual ~Type();
        bool isVoidTy() { return _typeID == VOID_TYPE_ID; }
        bool isLabel() { return _typeID == LABEL_TYPE_ID; }
        bool isInteger() { return _typeID == INTEGER_TYPE_ID; }
        bool isArray() { return _typeID == ARRAY_TYPE_ID; }
        bool isFunction() { return _typeID == FUNCTION_TYPE_ID; }
        bool isPointer() { return _typeID == POINTER_TYPE_ID; }
        explicit Type(LLVMContextPtr context, TypeID typeId)
                : _context(context), _typeID(typeId) {}
        virtual std::string output() = 0;
    private:
        TypeID _typeID;
        LLVMContextPtr _context;
    };

    class VoidType : public Type {
    public:
        explicit VoidType(LLVMContextPtr context): Type(context, INTEGER_TYPE_ID) {}
        std::string output() override {
            return "";
        }
    };

    class IntegerType : public Type {
        unsigned int bitwidth;
    public:
        explicit IntegerType(LLVMContextPtr context, unsigned int bytes)
               : Type(context, INTEGER_TYPE_ID), bitwidth(bytes) {}
        std::string output() override {
            return "";
        }
    };

    class PointerType: public Type {
        TypePtr _basicType;
    public:
        explicit PointerType(LLVMContextPtr context, TypePtr basicType)
               : Type(context, POINTER_TYPE_ID), _basicType(basicType) {}
        std::string output() override {
            return "";
        }
    };

    class FunctionType: public Type {
        TypePtr _retType;
        std::vector<TypePtr> _argTypes;
    public:
        explicit FunctionType(LLVMContextPtr context, TypePtr retType, std::vector<TypePtr>& argTypes)
                : Type(context, FUNCTION_TYPE_ID), _retType(retType), _argTypes(argTypes) {}
        std::string output() override {
            return "";
        }
    };
}

#endif //TYPE_HPP
