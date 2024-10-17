#include <fstream>
#include <iostream>

#include "libs/tang/lexer.hpp"
#include "libs/tang/token.hpp"
#include "libs/tang/error.hpp"
#include "libs/tang/parser1.hpp"
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
    std::string lexerOut = "lexer.txt";
    std::string parserOut = "parser.txt";
    std::string errOut = "error.txt";

    std::ifstream infile(input, std::ios::in);
    std::ofstream lexerStream;
    std::ofstream parserStream;
    std::ofstream erroutStream;

    lexerStream.open(lexerOut, std::ios::out);
    parserStream.open(parserOut, std::ios::out);
    erroutStream.open(errOut, std::ios::out);

    auto reporter = ErrorReporter(erroutStream);
    auto lexer = Lexer(infile, input, lexerStream, reporter);
    auto parser = Parser1(lexer, reporter, parserStream);
    parser.parse();

    return 0;
}