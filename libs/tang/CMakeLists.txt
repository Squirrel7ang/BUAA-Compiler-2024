set(LIBRARY_NAME tang)

add_library(${LIBRARY_NAME}
        Lexer.hpp
        Lexer.cpp
        Token.hpp
        Token.cpp
        Parser1.hpp
        Parser1.cpp
        Visitor.hpp
        Visitor.cpp
        ErrorReporter.hpp
        ErrorReporter.cpp
        Ast.hpp
        LoopStack.hpp
        Symbol.hpp
        Symbol.cpp
        Evaluate.cpp
        LLVMVisitor.cpp
        Ast.cpp
)

target_link_libraries(${LIBRARY_NAME} PUBLIC llvm)
target_include_directories(${LIBRARY_NAME} PUBLIC llvm)