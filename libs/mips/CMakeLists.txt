set(LIBRARY_NAME mips)

add_library(${LIBRARY_NAME}
        MipsCommon.hpp
        Imm.hpp
        Imm.cpp
        Regs.hpp
        Regs.cpp
        Insts.hpp
        Insts.cpp
        Asm.hpp
        Controller.hpp
)
target_link_libraries(${LIBRARY_NAME} PUBLIC llvm)

target_include_directories(${LIBRARY_NAME} PUBLIC llvm)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
