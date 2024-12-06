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
        std::set<VariablePtr> _graph;
        std::vector<ConflictEdge> _edges;
        VarTablePtr _varTable;
    public:
        static ConflictGraphPtr New(VarTablePtr varTable);
        void insertEdges(llvm::ConflictEdges& edges);
    private:
        explicit ConflictGraph(VarTablePtr& varTable);
    };
}

#endif //CONFLICTGRAPH_HPP
