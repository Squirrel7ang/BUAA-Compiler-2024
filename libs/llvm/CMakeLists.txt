set(LIBRARY_NAME llvm)

add_library(${LIBRARY_NAME}
        codeGen.hpp
        codeGen.cpp
        Symbol.cpp
        Symbol.hpp
)