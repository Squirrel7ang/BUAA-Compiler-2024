set(LIBRARY_NAME llvm)

add_library(${LIBRARY_NAME}
        codeGen.hpp
        codeGen.cpp
)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})