//
// Created by tang on 12/6/24.
//

#ifndef CONFLICTGRAPH_HPP
#define CONFLICTGRAPH_HPP

#include <utility>
#include <map>
#include <set>

#include "MipsCommon.hpp"
#include "IR/Common.hpp"

namespace mips {
    using ConflictEdge =
        std::pair<VariablePtr, VariablePtr>;

    class ConflictGraph {
    private:
        enum SaveRegAllocStatus {
            SRAS_NONE,
            SRAS_ALLOCATED,
        };
    private:
        StackPtr _stack;
        std::set<VariablePtr> _graph;
        std::vector<ConflictEdge> _edges;
        std::set<VariablePtr> _vars;
        VarTablePtr _varTable;
        SaveRegTablePtr _saveRegTable;
        std::map<VariablePtr, SaveRegAllocStatus> _allocStatus;
        std::map<VariablePtr, MipsRegPtr> _dyeInfo;
        std::map<VariablePtr, std::set<VariablePtr>> _deletedEdges;
        std::vector<VariablePtr> _deleteSequence;
    public:
        static ConflictGraphPtr New(VarTablePtr, SaveRegTablePtr&);
        explicit ConflictGraph(VarTablePtr& varTable, SaveRegTablePtr&);
        void insertEdges(llvm::ConflictEdges& edges);
        void dyeEveryNode();
        void setStack(StackPtr stack);
    private:
        void deleteVariable(VariablePtr vp);
        void setVarAllocStatus(VariablePtr vp, SaveRegAllocStatus newStatus);
        MipsRegPtr getFreeReg(std::set<VariablePtr> vars);
        void insertBack(VariablePtr vp);
        void dyeVar(const std::shared_ptr<Variable> & var);
    };
}

#endif //CONFLICTGRAPH_HPP
