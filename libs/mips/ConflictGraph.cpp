//
// Created by tang on 12/6/24.
//

#include "ConflictGraph.hpp"

#include "SaveRegTable.hpp"
#include "StackSlot.hpp"
#include "VarTable.hpp"

namespace mips {
    ConflictGraph::ConflictGraph(VarTablePtr& varTable, SaveRegTablePtr& srtp, StackPtr sp)
        : _varTable(varTable), _saveRegTable(srtp) {
        for (auto pair = varTable->begin(); pair != varTable->end(); ++pair) {
            if ((*pair).first->is(llvm::ALLOCA_INST_T)) {
                continue;
            }
            auto var = (*pair).second;
            _vars.insert(var);
        }
    }

    ConflictGraphPtr ConflictGraph::New(VarTablePtr varTable, SaveRegTablePtr& srtp, StackPtr sp) {
        return std::make_shared<ConflictGraph>(varTable, srtp, sp);
    }

    void ConflictGraph::insertEdges(llvm::ConflictEdges& edges) {
        for (auto& edge: edges) {
            if (edge.first->is(llvm::ALLOCA_INST_T) ||
                edge.second->is(llvm::ALLOCA_INST_T)) {
                continue;
            }
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

    void ConflictGraph::deleteVariable(VariablePtr vp) {
        _vars.erase(vp);
        _deletedEdges.insert({vp, vp->next});
        _deleteSequence.push_back(vp);

        for (const auto& v: vp->next) {
            v->removeConflictVar(vp);
        }
        vp->next.clear();
    }

    void ConflictGraph::setVarAllocStatus(VariablePtr vp, SaveRegAllocStatus newStatus) {
        if (_allocStatus.contains(vp)) {
            _allocStatus[vp] = newStatus;
        }
        else {
            _allocStatus.insert({vp, newStatus});
        }
    }

    /**
     * this function checks whether there are available color to dye.
     * If so, return the available color(reg), else return nullptr
     * @param vars the surrounded variables.
     * @return available register or null ptr if none is available.
     */
    MipsRegPtr ConflictGraph::getFreeReg(std::set<VariablePtr> vars) {
        std::set<MipsRegPtr> availableRegs = _saveRegTable->_saveRegPool;
        for (auto& var: vars) {
            if (_allocStatus[var] == SRAS_ALLOCATED) {
                MipsRegPtr& color = _dyeInfo[var];
                availableRegs.erase(color);
            }
        }
        if (availableRegs.empty()) {
            return nullptr;
        }
        else {
            return *(availableRegs.begin());
        }
    }

    void ConflictGraph::insertBack(VariablePtr vp) {
        std::set<VariablePtr>& edges = _deletedEdges[vp];
        for (const auto& v: edges) {
            v->addConflictVar(vp);
            vp->addConflictVar(v);
        }
    }

    void ConflictGraph::dyeVar(const VariablePtr &var) {
        auto reg = getFreeReg(var->next);
        if (reg == nullptr) {
            auto slot = _stack->allocateSlot(var->size());
            var->setLocation(slot);
        }
        else {
            var->setLocation(reg);
            _saveRegTable->use(reg);
        }
    }

    void ConflictGraph::dyeEveryNode() {
        // remove the node whose degree is less than the number of save registers.
        bool flag = true;
        while (flag) {
            flag = false;
            for (const auto & var : _vars) {
                if (var->next.size() < _saveRegTable->size()) {
                    setVarAllocStatus(var, SRAS_ALLOCATED);
                    deleteVariable(var);
                    flag = true;
                }
            }
        }

        // remove the rest of the node except the first one
        auto it = _vars.begin();
        ++it;
        for (; it != _vars.end(); ++it) {
            setVarAllocStatus(*it, SRAS_NONE);
            deleteVariable(*it);
        }

        // mark the last variable to be allocated.
        auto lastVar = *(_vars.begin());
        setVarAllocStatus(lastVar, SRAS_ALLOCATED);

        // now insert them back and dye each and every node.
        dyeVar(lastVar);
        for (auto& var: _deleteSequence) {
            insertBack(var);
            dyeVar(var);
        }
    }
}
