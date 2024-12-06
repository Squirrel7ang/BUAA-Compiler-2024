//
// Created by tang on 12/6/24.
//

#include "ConflictGraph.hpp"

namespace mips {
    void ConflictGraph::insertEdges(llvm::ConflictEdges& edges) {
        for (auto& edge: edges) {
            _edges.push_back(edge);

            auto node1 = edge.first;
            auto node2 = edge.second;

            if (_map.contains(node1))
                _map[node1].push_back(edge.second);
            else {
                ConflictNode n = ConflictNode(node1, node2);
                _map.insert({node1, n });
            }

            if (_map.contains(node2))
                _map[node2].push_back(edge.second);
            else {
                ConflictNode n = ConflictNode(node2, node1);
                _map.insert({node2, n });
            }
        }
    }
}
