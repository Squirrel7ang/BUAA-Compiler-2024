set(LIBRARY_NAME tang)

add_library(${LIBRARY_NAME}
        lexer.hpp
        lexer.cpp
        token.hpp
        token.cpp
        parser.hpp
        parser.cpp
        parser1.cpp
        parser1.hpp
        ast.hpp)