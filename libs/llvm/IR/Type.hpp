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
        explicit Type(LLVMContextPtr& context, TypeID typeId)
                : _context(context), _typeID(typeId) {}
        virtual void print(std::ostream& out) = 0;

    protected:
        TypeID _typeID;
        LLVMContextPtr _context;
    };

    class LabelType : public Type {
    public:
        explicit LabelType(LLVMContextPtr& context): Type(context, LABEL_TYPE_ID) {}
        void print(std::ostream& out) override {
            out << "label";
        }
    };

    class VoidType : public Type {
    public:
        explicit VoidType(LLVMContextPtr& context): Type(context, INTEGER_TYPE_ID) {}
        void print(std::ostream& out) override {
            out << "void";
        }
    };

    class IntegerType : public Type {
        unsigned int bitwidth;
    public:
        explicit IntegerType(LLVMContextPtr& context, unsigned int bits)
               : Type(context, INTEGER_TYPE_ID), bitwidth(bits) {}
        void print(std::ostream& out) override {
            out << 'i' << bitwidth;
        }
    };

    class ArrayType: public Type {
        TypePtr _basicType;
        unsigned int _length;
    public:
        explicit ArrayType(LLVMContextPtr& context, TypePtr basicType, int length)
                : Type(context, ARRAY_TYPE_ID), _basicType(basicType) { }
        void print(std::ostream& out) override {
            out << "[" << _length << " x ";
            _basicType->print(out);
            out << "]";
        }
    };

    class PointerType: public Type {
        TypePtr _basicType;
    public:
        explicit PointerType(LLVMContextPtr& context, TypePtr basicType)
               : Type(context, POINTER_TYPE_ID), _basicType(basicType) {}
        void print(std::ostream& out ) override {
            out << "ptr";
        }
    };

    class FunctionType: public Type {
    public:
        TypePtr _retType;
        std::vector<TypePtr> _argTypes;
    public:
        explicit FunctionType(LLVMContextPtr& context, TypePtr retType, std::vector<TypePtr>& argTypes)
                : Type(context, FUNCTION_TYPE_ID), _retType(retType), _argTypes(argTypes) {}
        void print(std::ostream& out) override {
            out << "FunctionType";
        }
    };
}

#endif //TYPE_HPP
