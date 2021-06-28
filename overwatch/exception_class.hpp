#ifndef EXCEPTION_CLASS_H
#define EXCEPTION_CLASS_H

#include <exception>

class ExceptionClass{
    protected:
        std::exception_ptr pException;
    public:
        ExceptionClass() { this->pException = nullptr; }
        const std::exception_ptr getException() const noexcept;
};

#endif