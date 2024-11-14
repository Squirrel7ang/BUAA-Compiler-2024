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
        explicit ConstantData(LLVMContextPtr context, TypePtr ty,
                              vector<int>& data, int length)
                : Constant(context, ty, CONSTANT_DATA_T), _data(data), _length(length) {
            for (int i = _data.size(); i < _length; i++) {
                _data.push_back(0);
            }
        }
        explicit ConstantData(LLVMContextPtr context, TypePtr ty, int data)
                : Constant(context, ty, CONSTANT_DATA_T), _data(data) { }
    };
}

#endif //CONSTANTDATA_HPP
