set(LIBRARY_NAME mips)

add_library(${LIBRARY_NAME}
        MipsCommon.hpp
        MipsImm.hpp
        MipsImm.cpp
        Regs.hpp
        Regs.cpp
        Insts.hpp
        Insts.cpp
        MipsAsmGen.hpp
        Controller.hpp
        TmpRegTable.hpp
        SaveRegTable.hpp
        VarTable.hpp
        VarTable.cpp
        MipsAsmGen.cpp
        TmpRegTable.cpp
        StackSlot.hpp
        StackSlot.cpp
        MipsTranslator.hpp
        MipsTranslator.cpp
        MipsRegAllocator.cpp
        MipsRegAllocator.hpp
        VarLocation.cpp
        VarLocation.hpp
        ConflictGraph.cpp
        ConflictGraph.hpp
        SaveRegTable.cpp
)
target_link_libraries(${LIBRARY_NAME} PUBLIC llvm)

target_include_directories(${LIBRARY_NAME} PUBLIC llvm)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
