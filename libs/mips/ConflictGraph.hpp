//
// Created by tang on 12/6/24.
//

#ifndef CONFLICTGRAPH_HPP
#define CONFLICTGRAPH_HPP

#include <utility>
#include <map>

#include "IR/Common.hpp"

namespace mips {
    class ConflictNode {
        llvm::InstructionPtr node;
        std::vector<llvm::InstructionPtr> next;
    public:
        explicit ConflictNode(llvm::InstructionPtr inst) : node(inst) { }
        explicit ConflictNode(llvm::InstructionPtr inst,
                              llvm::InstructionPtr target) {

        }
        void push_back(llvm::InstructionPtr inst) { next.push_back(inst); }
    };

    using ConflictEdge =
        std::pair<llvm::InstructionPtr, llvm::InstructionPtr>;

    class ConflictGraph {
        std::map<llvm::InstructionPtr, ConflictNode> _map;
        std::vector<ConflictEdge> _edges;
    public:
        void insertEdges(llvm::ConflictEdges& edges);
    };
}

#endif //CONFLICTGRAPH_HPP
