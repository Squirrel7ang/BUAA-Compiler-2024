//
// Created by tang on 12/6/24.
//

#include "ConflictGraph.hpp"

#include "VarTable.hpp"

namespace mips {
    ConflictGraph::ConflictGraph(VarTablePtr& varTable): _varTable(varTable) { }

    ConflictGraphPtr ConflictGraph::New(VarTablePtr varTable) {
        return std::make_shared<ConflictGraph>(varTable);
    }

    void ConflictGraph::insertEdges(llvm::ConflictEdges& edges) {
        for (auto& edge: edges) {
            auto var0 = _varTable->findVar(edge.first);
            auto var1 = _varTable->findVar(edge.second);
            _edges.push_back({var0, var1});

            if (!_graph.contains(var0))
                _graph.insert(var0);
            if (!_graph.contains(var1))
                _graph.insert(var1);

            var1->addConflictVar(var0);
            var0->addConflictVar(var1);
        }
    }
}
