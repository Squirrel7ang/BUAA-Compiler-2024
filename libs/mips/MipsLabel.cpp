//
// Created by tang on 12/11/24.
//

#include "MipsLabel.hpp"

#include <cassert>

#include "IR/Module.hpp"

namespace mips {

    MipsLabelPtr MipsLabel::New(std::string &name) {
        return std::make_shared<MipsLabel>(name);
    }

    MipsLabel::MipsLabel(std::string &name)
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
                auto name = "L" + std::to_string(bb->getIndex());
                auto&& label = MipsLabel::New(name);
                _table.insert({*b, label});
            }
        }
    }

    MipsLabelPtr LabelTable::findLabel(llvm::BasicBlockPtr bb) {
        if (_table.contains(bb)) {
            return _table[bb];
        }
        else {
            assert(0);
        }
    }

    std::map<llvm::BasicBlockPtr, MipsLabelPtr>::iterator LabelTable::begin() {
        return _table.begin();
    }

    std::map<llvm::BasicBlockPtr, MipsLabelPtr>::iterator LabelTable::end() {
        return _table.end();
    }

    void LabelTable::addLabel(llvm::BasicBlockPtr bb, MipsLabelPtr label) {
        _table.insert({bb, label});
    }
}

#include "MipsLabel.hpp"
