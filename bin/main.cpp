#include <fstream>
#include <iostream>

#include "libs/tang/lexer.hpp"
#include "libs/tang/token.hpp"
#include <string>
using namespace tang;

int main(int argc, char *argv[]) {
    std::string input;
    if (argc > 1) {
        input = std::string(argv[1]);
    }
    else {
        input = "testfile.txt";
    }
    std::string corout = "lexer.txt";
    std::string errout = "error.txt";

    std::ifstream infile(input, std::ios::in);
    std::ofstream coroutFile;
    coroutFile.open(corout, std::ios::out);
    std::ofstream erroutFile;
    erroutFile.open(errout, std::ios::out);

    auto lexer = Lexer(infile, input);

    while (1) {
        Token token = lexer.nextToken();
        if (token.isEOF())
            break;
        else if (token.getType() == TK_COMMENT) {
            continue;
        }
        else if (token.isUnknown()) {
            unsigned line = token.getLin();
            erroutFile << line << ' ' << 'a' << std::endl;
            // std::cout << line << ' ' << 'a' << std::endl;
            break;
        }
        else {
            coroutFile << token.toString() << ' ' << token.getContent() << std::endl;
            // std::cout << token.toString() << ' ' << token.getContent() << std::endl;
        }
    }

    return 0;
}