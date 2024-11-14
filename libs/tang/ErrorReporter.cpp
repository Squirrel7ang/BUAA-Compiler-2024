//
// Created by tang on 10/16/24.
//

#include "ErrorReporter.hpp"

namespace tang {

    void ErrorReporter::report(const unsigned int lineNumber, const char errorType) {
        errorMessages.push_back(ErrorMessage(lineNumber, errorType));
    }

    void ErrorReporter::_printOneMessage(ErrorMessage& msg) {
        _errOutput << msg;
    }

    void ErrorReporter::printAll() {
        std::stable_sort(errorMessages.begin(), errorMessages.end());

        if (errorMessages.size() > 0) {
            _printOneMessage(errorMessages.at(0));
        }
        if (errorMessages.size() > 1) {
            for (int i = 0; i < errorMessages.size()-1; i++) {
                int j = i+1;
                auto& msg1 = errorMessages.at(i);
                auto& msg2 = errorMessages.at(j);
                if (msg1 != msg2) {
                    _printOneMessage(msg2);
                }
            }
        }
    }
} // namespace tang