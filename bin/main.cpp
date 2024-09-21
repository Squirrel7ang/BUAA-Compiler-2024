#include <fstream>

#include "tang/lexer.h"
#include "tang/token.h"
#include <string>
using namespace tang;

int main(int argc, char *argv[]) {
    std::string input = "testfile.txt";
    std::string corout = "lexer.txt";
    std::string errout = "error.txt";

    std::ifstream infile(input, std::ios::in);
    std::ofstream coroutFile;
    coroutFile.open(corout, std::ios::out);
    std::ofstream erroutFile;
    erroutFile.open(errout, std::ios::out);

    auto lexer = Lexer(infile, input);

    while (1) {
        Token token = lexer.readNextToken();
        if (token.isEOF())
            break;
        else if (token.isUnknown()) {
            unsigned line = token.getLin();
            erroutFile << line << ' ' << 'a' << std::endl;
            break;
        }
        else {
            coroutFile << token.getType() << ' ' << token.getContent() << std::endl;
        }
    }

    return 0;
}