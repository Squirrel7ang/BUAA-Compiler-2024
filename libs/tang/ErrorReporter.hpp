//
// Created by tang on 10/16/24.
//

#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>
#include <vector>
#include <algorithm>

namespace tang {
    class ErrorMessage {
        unsigned int _lin;
        char _errorType;
    public:
        explicit ErrorMessage(unsigned int lin, char type):
            _lin(lin), _errorType(type) { }
        bool operator<(const ErrorMessage & op) const {
            return this->_lin < op._lin;
        }
        bool operator==(const ErrorMessage& op) const {
            return this->_lin == op._lin &&
                    this->_errorType == op._errorType;
        }
        bool operator!=(const ErrorMessage &op) const {
            return !(*this == op);
        }
        friend std::ostream& operator<<(std::ostream& out, const ErrorMessage& msg) {
            out << msg._lin << " " << msg._errorType << std::endl;
            return out;
        }
    };

    class ErrorReporter {
        std::ostream& _errOutput;
        std::vector<ErrorMessage> errorMessages;
        void _printOneMessage(ErrorMessage& msg);
    public:
        explicit ErrorReporter(std::ostream& errorOutput) : _errOutput(errorOutput) {}
        void report(unsigned int lineNumber, char errorType);
        void printAll();
        bool error() { return !errorMessages.empty(); }
    };
} // namespace tang

#endif //ERROR_HPP
