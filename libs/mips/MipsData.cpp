//
// Created by tang on 12/10/24.
//

#include "MipsData.hpp"

#include "IR/Module.hpp"

namespace mips {
    MipsDataPtr MipsData::New(llvm::GlobalVariablePtr var) {
        return std::make_shared<MipsData>(var);
    }

    MipsDataPtr MipsData::New(llvm::GlobalStringPtr str) {
        return std::make_shared<MipsData>(str);
    }

    MipsData::MipsData(llvm::GlobalVariablePtr gvp) : MipsImm(0), _name(gvp->getName()) {
        // _dataType
        llvm::IntegerTypePtr ity;
        if (gvp->getType()->isInteger()) {
            assert(0);
        }
        else if (gvp->getType()->isPointer()) {
            auto pty = std::dynamic_pointer_cast<llvm::PointerType>(gvp->getType());
            if (pty->getPtrBasicType()->isInteger()) {
                auto tmpTy = pty->getPtrBasicType();
                ity = std::dynamic_pointer_cast<llvm::IntegerType>(tmpTy);
            }
            else if (pty->getPtrBasicType()->isArray()) {
                auto&& aty = std::dynamic_pointer_cast<llvm::ArrayType>(pty->getPtrBasicType());
                ity = std::dynamic_pointer_cast<llvm::IntegerType>(aty->getBasicType());
            }
        }
        else
            assert(0);

        int size = ity->getSize();
        if (size == 1) _dataType = MDT_BYTE;
        else if (size == 4) _dataType = MDT_WORD;
        else assert(0);

        // initVals
        _initVals = gvp->getInitVal()->_data;
    }

    MipsData::MipsData(llvm::GlobalStringPtr gsp)
        : MipsImm(0), _name(gsp->getName()), _dataType(MDT_STR) {

        std::string str = gsp->getMipsData();
        for (auto ch: str) {
            _initVals.push_back((int) ch);
        }
    }

    void MipsData::print(std::ostream &out) const {
        out << "  " << _name << ": ";
        if (_dataType == MDT_WORD) {
            out << ".word ";
            for (const auto& i: _initVals) {
                out << i << ' ';
            }
        }
        else if (_dataType == MDT_BYTE) {
            out << ".byte ";
            for (const auto& i: _initVals) {
                out << i << ' ';
            }
        }
        else if (_dataType == MDT_STR) {
            out << ".asciiz \"";
            for (const auto& i: _initVals) {
                char ch = (char) i;
                if (ch == '\n') out << "\n";
                else out << ch ;
            }
            out << "\"";
        }

        out << std::endl;
    }

    DataTablePtr DataTable::New(llvm::ModulePtr module) {
        return std::make_shared<DataTable>(module);
    }

    DataTable::DataTable(llvm::ModulePtr module) {
        auto&& globalVarBegin = module->globalVarBegin();
        auto&& globalVarEnd = module->globalVarEnd();
        for (auto it = globalVarBegin; it != globalVarEnd; ++it) {
            addGlobalValue(*it, MipsData::New(*it));
        }

        auto&& stringBegin = module->stringBegin();
        auto&& stringEnd = module->stringEnd();
        for (auto it = stringBegin; it != stringEnd; ++it) {
            addGlobalValue(*it, MipsData::New(*it));
        }
    }

    MipsDataPtr DataTable::findData(llvm::GlobalValuePtr gvp) {
        if (_table.contains(gvp)) {
            return _table[gvp];
        }
        else {
            return nullptr;
        }
    }

    std::map<llvm::GlobalValuePtr, MipsDataPtr>::iterator DataTable::begin() {
        return _table.begin();
    }

    std::map<llvm::GlobalValuePtr, MipsDataPtr>::iterator DataTable::end() {
        return _table.end();
    }

    void DataTable::addGlobalValue(llvm::GlobalVariablePtr gvp, MipsDataPtr mdp) {
        _table.insert({gvp, mdp});
    }

    void DataTable::addGlobalValue(llvm::GlobalStringPtr gsp, MipsDataPtr mdp) {
        _table.insert({gsp, mdp});
    }
}
