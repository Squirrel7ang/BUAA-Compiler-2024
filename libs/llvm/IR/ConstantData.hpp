//
// Created by tang on 11/14/24.
//

#ifndef CONSTANTDATA_HPP
#define CONSTANTDATA_HPP

#include "Common.hpp"
#include "Constant.hpp"

namespace llvm {
    class ConstantData : public Constant {
    private:
        int _length;
    public:
        vector<int> _data;

    public:
        explicit ConstantData(LLVMContextPtr& context, TypePtr ty,
                              vector<int>& data, int length, bool pad)
                : Constant(context, ty, CONSTANT_DATA_T), _data(data), _length(length) {
            if (pad) {
                for (int i = data.size(); i < _length; i++) {
                    _data.push_back(0);
                }
            }
        }
        explicit ConstantData(LLVMContextPtr& context, TypePtr ty, int data)
                : Constant(context, ty, CONSTANT_DATA_T), _length(-1) {
            _data.push_back(data);
        }
    public:
        int size() { return _length; }
        void print(std::ostream& out) override {
            if (_length < 0) {
                out << _data.at(0);
            }
            else {
                out << "[";
                auto ty = std::static_pointer_cast<ArrayType>(_type)->getBasicType();
                for (int i = 0; i < _length - 1; i++) {
                    ty->print(out);
                     out << " " << _data.at(i) << ", ";
                }
                ty->print(out);
                out << " " << _data.at(_length - 1) << "]";
            }
        }
        void printRef(std::ostream &out) override {
            print(out);
        }

        int getVal(unsigned int i) {
            assert(i < _length);
            return _data[i];
        }
    };
}

#endif //CONSTANTDATA_HPP
