//
// Created by tang on 11/7/24.
//

#ifndef TYPE_HPP
#define TYPE_HPP

#include <cassert>
#include <memory>

#include "Common.hpp"

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
        TypeID getID() { return _typeID; }
        explicit Type(TypeID typeId)
                : _typeID(typeId) {}
        virtual void print(std::ostream& out) { }
        virtual bool equals(TypePtr type) {
            return type->getID() == _typeID;
        }

    protected:
        TypeID _typeID;
        // LLVMContextPtr _context;
    };

    class LabelType : public Type {
    public:
        explicit LabelType(): Type(LABEL_TYPE_ID) {}
        void print(std::ostream& out) override {
            out << "label";
        }
    };

    class VoidType : public Type {
    public:
        explicit VoidType(): Type(VOID_TYPE_ID) {}
        void print(std::ostream& out) override {
            out << "void";
        }
    };

    class IntegerType : public Type {
        unsigned int bitwidth;
    public:
        explicit IntegerType(unsigned int bits)
               : Type(INTEGER_TYPE_ID), bitwidth(bits) {}
        void print(std::ostream& out) override {
            out << 'i' << bitwidth;
        }
        unsigned int getBits() { return bitwidth; }
        bool equals(TypePtr type) override {
            return type->isInteger() &&
                   (std::static_pointer_cast<IntegerType>(type)->getBits() == bitwidth);
        }
    };

    class ArrayType: public Type {
        TypePtr _basicType;
        unsigned int _length;
    public:
        explicit ArrayType(TypePtr basicType, int length)
                : Type(ARRAY_TYPE_ID), _basicType(basicType), _length(length) { }
        void print(std::ostream& out) override {
            out << "[" << _length << " x ";
            _basicType->print(out);
            out << "]";
        }
        TypePtr getBasicType() { return _basicType; }
        unsigned int getLength() { return _length; }
        bool equals(TypePtr type) override {
            if (!type->isArray())
                return false;
            auto&& _type = std::static_pointer_cast<ArrayType>(type);
            return _type->getLength() == _length &&
                   _basicType->equals(_type->getBasicType());
        }
    };

    class PointerType: public Type {
        TypePtr _basicType;
    public:
        explicit PointerType(TypePtr basicType)
               : Type(POINTER_TYPE_ID), _basicType(basicType) {}
        void print(std::ostream& out ) override {
            _basicType->print(out);
            out << '*';
            // out << "ptr";
        }
        TypePtr getBasicType() { return _basicType; }
        bool equals(TypePtr type) override {
            return type->isPointer() &&
                   _basicType->equals(std::static_pointer_cast<PointerType>(type)->getBasicType());
        }
    };

    class FunctionType: public Type {
    public:
        TypePtr _retType;
        std::vector<TypePtr> _argTypes;
    public:
        explicit FunctionType(TypePtr retType, std::vector<TypePtr>& argTypes)
                : Type(FUNCTION_TYPE_ID), _retType(retType), _argTypes(argTypes) {}
        void print(std::ostream& out) override {
            out << "FunctionType";
        }
        bool equals(TypePtr type) override {
            if (!type->isFunction())
                return false;
            auto fty = std::static_pointer_cast<FunctionType>(type);
            if (!_retType->equals(fty->_retType))
                return false;
            if (fty->_argTypes.size() != _argTypes.size())
                return false;
            for (int i = 0; i < _argTypes.size(); i++) {
                auto&& ty0 = _argTypes.at(i);
                auto&& ty1 = fty->_argTypes.at(i);
                if (!ty0->equals(ty1))
                    return false;
            }
            return true;
        }
    };
}

#endif //TYPE_HPP
