#ifndef EXCEPTION_CLASS_H
#define EXCEPTION_CLASS_H

#include <exception>
#include <ios>

class ExceptionClass{
protected:
    std::exception_ptr pException;
public:
    ExceptionClass() { this->pException = nullptr; }
    const std::exception_ptr getException() const noexcept {
        return this->pException;
    };
	bool good() const noexcept { 
		return this->pException == nullptr; 
	}
};

class ErrorStrException : public std::exception {
	char errstr[256];
	char detail[512];
public:
	ErrorStrException() = delete;
	ErrorStrException(const char* errstr, const char* detail) { 
		memcpy_s(this->errstr, sizeof(this->errstr), errstr, sizeof(errstr));
		memcpy_s(this->detail, sizeof(this->detail), detail, sizeof(detail));
	}
	const char* what() const {
		std::string tmp;
		tmp.push_back(*errstr);
		tmp.push_back(*" : ");
		tmp.push_back(*detail);
		return tmp.c_str();
	}
};

#endif