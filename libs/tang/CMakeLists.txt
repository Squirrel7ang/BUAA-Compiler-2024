set(LIBRARY_NAME tang)

add_library(${LIBRARY_NAME}
        lexer.hpp
        lexer.cpp
        token.hpp
        token.cpp
        parser1.hpp
        parser1.cpp
        visitor.hpp
        visitor.cpp
        error.hpp
        error.cpp
        ast.hpp
        loopStack.hpp
        symbol.hpp
        symbol.cpp
)
target_link_libraries(${LIBRARY_NAME} PUBLIC llvm)

target_include_directories(${LIBRARY_NAME} PUBLIC llvm)
