#include "tang/lexer.hpp"
#include "tang/token.hpp"
#include <istream>

namespace tang {
    Lexer::Lexer(std::istream& input) : input(input) {}

}