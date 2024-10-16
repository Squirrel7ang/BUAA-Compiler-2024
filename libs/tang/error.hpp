//
// Created by tang on 10/16/24.
//

#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>

namespace tang {

    class ErrorReporter {
        std::ostream& _errOutput;
    public:
        explicit ErrorReporter(std::ostream& errorOutput) : _errOutput(errorOutput) {}
        void report(const unsigned int lineNumber, const char errorType) {
            _errOutput << lineNumber << " " << errorType << std::endl;
        }
    };
} // namespace tang

#endif //ERROR_HPP
