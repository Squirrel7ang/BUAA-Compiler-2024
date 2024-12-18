//
// Created by tang on 12/11/24.
//

#include "MipsLabel.hpp"

#include <cassert>

#include "IR/Module.hpp"

namespace mips {

    MipsLabelPtr MipsLabel::New(const std::string &name) {
        return std::make_shared<MipsLabel>(name);
    }

    MipsLabel::MipsLabel(const std::string &name)
        :MipsImm(0), _name(name) {
    }

    void MipsLabel::print(std::ostream &out) const {
        out << _name << ':' << std::endl;
    }

    LabelTablePtr LabelTable::New(llvm::ModulePtr m) {
        return std::make_shared<LabelTable>(m);
    }

    LabelTable::LabelTable(llvm::ModulePtr module) {
        auto&& funcBegin = module->functionBegin();
        auto&& funcEnd = module->functionEnd();
        for (auto f = funcBegin; f != funcEnd; ++f) {
            auto&& blockBegin = (*f)->blockBegin();
            auto&& blockEnd = (*f)->blockEnd();
            for (auto b = blockBegin; b != blockEnd; ++b) {
                auto& bb = *b;
                std::string name = "L" + std::to_string(bb->getIndex());
                auto&& label = MipsLabel::New((*f)->getName() + name);
                _basicBlockTable.insert({*b, label});
            }
        }
        for (auto f = funcBegin; f != funcEnd; ++f) {
            auto&& label = MipsLabel::New("L" + (*f)->getName());
            _funcTable.insert({*f, label});
        }
    }

    MipsLabelPtr LabelTable::findLabel(llvm::BasicBlockPtr bb) {
        if (_basicBlockTable.contains(bb)) {
            return _basicBlockTable[bb];
        }
        else {
            assert(0);
        }
    }

    MipsLabelPtr LabelTable::findLabel(llvm::FunctionPtr f) {
        if (_funcTable.contains(f)) {
            return _funcTable[f];
        }
        else {
            assert(0);
        }
    }

    std::map<llvm::BasicBlockPtr, MipsLabelPtr>::iterator LabelTable::begin() {
        return _basicBlockTable.begin();
    }

    std::map<llvm::BasicBlockPtr, MipsLabelPtr>::iterator LabelTable::end() {
        return _basicBlockTable.end();
    }

    void LabelTable::addLabel(llvm::BasicBlockPtr bb, MipsLabelPtr label) {
        _basicBlockTable.insert({bb, label});
    }
}

#include "MipsLabel.hpp"
