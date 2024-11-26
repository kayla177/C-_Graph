#ifndef ILLEGAL_EXCEPTION_HPP
#define ILLEGAL_EXCEPTION_HPP

#include <exception>

class illegal_exception : public std::exception {
public:
    const char* what() const noexcept override {
        return "illegal argument";
    }
};

#endif
