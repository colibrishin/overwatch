#ifndef EXCEPTION_CLASS_H
#define EXCEPTION_CLASS_H

#include <exception>
#include <ios>

namespace Exceptions {
	class Exception : public std::exception {
		std::string errstr;
	private:
		void ToErrstr(const std::string explain, const std::string detail) {
			errstr.append(explain);
			errstr.append(" : ");
			errstr.append(detail);
		}
	public:
		Exception() = delete;
		Exception(const std::string explain, const std::string detail) {
			ToErrstr(explain, detail);
		}
		Exception(const std::ios::failure& e) {
			ToErrstr(e.what(), e.code().message());
		}
		Exception(const std::filesystem::filesystem_error& e) {
			ToErrstr(e.what(), e.path1().string() + " " + e.path2().string());
		}

		const char* what() const { return errstr.c_str(); }
	};

	class programming_error : public Exception {
	public:
		programming_error() = delete;
		programming_error(const char* detail) : Exception("Programming Error", detail) {}
	};

	class file_invalidate : public Exception {
	public:
		file_invalidate() = delete;
		file_invalidate(const char* detail) : Exception("File might have been corrupted.", detail) {}
	};
	
}


#endif