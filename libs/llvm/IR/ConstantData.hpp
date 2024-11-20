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
        vector<int> _data;
        int _length;
    public:
        explicit ConstantData(LLVMContextPtr& context, TypePtr ty,
                              vector<int>& data, int length)
                : Constant(context, ty, CONSTANT_DATA_T), _data(data), _length(length) {
            for (int i = _data.size(); i < _length; i++) {
                _data.push_back(0);
            }
        }
        explicit ConstantData(LLVMContextPtr& context, TypePtr ty, int data)
                : Constant(context, ty, CONSTANT_DATA_T), _length(-1) {
            _data.push_back(data);
        }
    public:
        void print(std::ostream& out) {
            if (_length < 0) {
                out << _data.at(0);
            }
            else {
                out << "[";
                for (int i = 0; i < _length - 1; i++) {
                    _type->print(out);
                     out << " " << _data.at(i) << ", ";
                }
                _type->print(out);
                out << " " << _data.at(_length - 1) << "]";
            }
        }
        void printRef(std::ostream &out) override {
            print(out);
        }
    };
}

#endif //CONSTANTDATA_HPP
