#include <fstream>
#include <iostream>

#include "libs/tang/Lexer.hpp"
#include "libs/tang/ErrorReporter.hpp"
#include "libs/tang/Parser1.hpp"
#include "libs/tang/Visitor.hpp"
#include <string>

#include "IR/IRPrinter.hpp"
#include "libs/mips/MipsAsmGen.hpp"
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
    std::string visitorOut = "symbol.txt";
    std::string errOut = "error.txt";
    std::string llvmOut = "llvm_ir.txt";
    std::string mipsOut = "mips.txt";

    std::ifstream infile(input, std::ios::in);
    std::ofstream lexerStream;
    std::ofstream parserStream;
    std::ofstream visitorStream;
    std::ofstream erroutStream;
    std::ofstream llvmStream;
    std::ofstream mipsStream;

    lexerStream.open(lexerOut, std::ios::out);
    parserStream.open(parserOut, std::ios::out);
    visitorStream.open(visitorOut, std::ios::out);
    erroutStream.open(errOut, std::ios::out);
    llvmStream.open(llvmOut, std::ios::out);
    mipsStream.open(mipsOut, std::ios::out);

    auto reporter = ErrorReporter(erroutStream);
    auto lexer = Lexer(infile, input, lexerStream, reporter);
    auto parser = Parser1(lexer, reporter, parserStream);
    auto compUnitPtr = parser.parse();
    auto visitor = Visitor(compUnitPtr, visitorStream, reporter);
    visitor.visit();
    if (reporter.error()) {
        reporter.printAll();
        return 0;
    }
    auto llvmModule = visitor.getLLVMModule();
    llvmModule->print(llvmStream);

    llvmModule->analyzeActiveVariable();
    auto asmGenerator = mips::MipsAsm(llvmModule);
    asmGenerator.generateMipsAsm();
    asmGenerator.print(mipsStream);
    // auto translator = asmGenerator.getTranslator();


    return 0;
}