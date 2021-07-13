#ifndef EXCEPTION_CLASS_H
#define EXCEPTION_CLASS_H

#include <exception>
#include <ios>

namespace Exceptions {
	namespace LoaderException {
		class LoaderException : public std::exception {
			std::string errstr;
		public:
			LoaderException() = delete;
			LoaderException(const std::string explain, const std::string detail) {
				errstr.append(explain);
				errstr.append(" : ");
				errstr.append(detail);
			}
			const char* what() const { return errstr.c_str(); }
		};

		class programming_error : public LoaderException {
		public:
			programming_error() = delete;
			programming_error(const char* detail) : LoaderException("Programming Error", detail) {}
		};

		class file_invalidate : public LoaderException {
		public:
			file_invalidate() = delete;
			file_invalidate(const char* detail) : LoaderException("File might have been corrupted.", detail) {}
		};
	}

	class ExceptionClass {
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
}


#endif