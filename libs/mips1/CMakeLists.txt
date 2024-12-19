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
        VarTable.hpp
        VarTable.cpp
        MipsAsmGen.cpp
        MipsTranslator.hpp
        MipsTranslator.cpp
        MipsData.cpp
        MipsData.hpp
        MipsLabel.cpp
        MipsLabel.hpp
        MipsLocation.cpp
        MipsLocation.hpp
)
target_link_libraries(${LIBRARY_NAME} PUBLIC llvm)
target_include_directories(${LIBRARY_NAME} PUBLIC llvm)
