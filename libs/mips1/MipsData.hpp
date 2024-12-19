//
// Created by tang on 12/10/24.
//

#ifndef MIPSDATA_HPP
#define MIPSDATA_HPP

#include <map>
#include <vector>
#include <string>
#include <ostream>

#include "IR/GlobalVariable.hpp"
#include "IR/GlobalString.hpp"
#include "IR/Common.hpp"

#include  "MipsCommon.hpp"
#include "MipsImm.hpp"

namespace mips {
    enum MipsDataType {
        MDT_STR,
        MDT_WORD,
        MDT_BYTE,
    };

    class MipsData: public MipsImm {
    private:
        std::vector<int> _initVals;
        std::string _name;
        MipsDataType _dataType;
    public:
        static MipsDataPtr New(llvm::GlobalVariablePtr);
        static MipsDataPtr New(llvm::GlobalStringPtr);
        explicit MipsData(llvm::GlobalVariablePtr);
        explicit MipsData(llvm::GlobalStringPtr gsp);

        bool isString() { return _dataType == MDT_STR; }
        bool isWord() { return _dataType == MDT_WORD; }
        bool isByte() { return _dataType == MDT_BYTE; }
        unsigned int size() { return _initVals.size(); }
        void print(std::ostream& out) const override;
    };

    class DataTable {
    private:
        std::map<llvm::GlobalValuePtr, MipsDataPtr> _table;
    public:
        static DataTablePtr New(llvm::ModulePtr);
        explicit DataTable(llvm::ModulePtr);

        MipsDataPtr findData(llvm::GlobalValuePtr);
        std::map<llvm::GlobalValuePtr, MipsDataPtr>::iterator begin();
        std::map<llvm::GlobalValuePtr, MipsDataPtr>::iterator end();
        unsigned int size() { return _table.size(); }
    private:
        void addGlobalValue(llvm::GlobalVariablePtr, MipsDataPtr);
        void addGlobalValue(llvm::GlobalStringPtr, MipsDataPtr);
    };
}

#endif //MIPSDATA_HPP
