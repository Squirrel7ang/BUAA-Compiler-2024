//
// Created by tang on 11/26/24.
//

#ifndef MIPSCOMMON_HPP
#define MIPSCOMMON_HPP

#include <memory>
#include <vector>

namespace mips {
    template <typename T>
    using u_ptr = std::unique_ptr<T>;

    template <typename T>
    using s_ptr = std::shared_ptr<T>;

    template <typename T>
    using vector = std::vector<T>;

    class MipsAsm;
    using MipsAsmPtr = s_ptr<MipsAsm>;

    class MipsController;
    using MipsControllerPtr = s_ptr<MipsController>;

    class MipsReg;
    using MipsRegPtr = s_ptr<MipsReg>;

    class Stack;
    using StackPtr = s_ptr<Stack>;

    class StackSlot;
    using StackSlotPtr = s_ptr<StackSlot>;

    class Variable;
    using VariablePtr = s_ptr<Variable>;

    class VarLocation;
    using VarLocationPtr = s_ptr<VarLocation>;

    class VarTable;
    using VarTablePtr = s_ptr<VarTable>;

    class TmpRegTable;
    using TmpRegTablePtr = s_ptr<TmpRegTable>;

    class SaveRegTable;
    using SaveRegTablePtr = s_ptr<SaveRegTable>;

    class MipsTranslator;
    using MipsTranslatorPtr = s_ptr<MipsTranslator>;

    class MipsRegAllocator;
    using MipsRegAllocatorPtr = s_ptr<MipsRegAllocator>;

    class ConflictGraph;
    using ConflictGraphPtr = s_ptr<ConflictGraph>;

    class MipsInst;
    using MipsInstPtr = s_ptr<MipsInst>;

    class IInst;
    using IInstPtr = s_ptr<IInst>;

    class JInst;
    using JInstPtr = s_ptr<JInst>;

    class RInst;
    using RInstPtr = s_ptr<RInst>;

    class MipsImm;
    using MipsImmPtr = s_ptr<MipsImm>;
}

#endif //MIPSCOMMON_HPP
