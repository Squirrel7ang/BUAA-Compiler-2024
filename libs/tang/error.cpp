//
// Created by tang on 10/16/24.
//

#include "error.hpp"

namespace tang {

    void ErrorReporter::report(const unsigned int lineNumber, const char errorType) {
        errorMessages.push_back(ErrorMessage(lineNumber, errorType));
    }

    void ErrorReporter::_printOneMessage(ErrorMessage& msg) {
        _errOutput << msg;
    }

    void ErrorReporter::printAll() {
        std::stable_sort(errorMessages.begin(), errorMessages.end());
        for (auto msg: errorMessages) {
            _printOneMessage(msg);
        }
    }
} // namespace tang