#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <filesystem>
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

	class add_snapshot_failed : public Exception {
	public:
		add_snapshot_failed() = delete;
		add_snapshot_failed(const char* detail) : Exception("Failed to add snapshot.", detail) {}
	};

	class create_game_failed : public Exception {
	public:
		create_game_failed() = delete;
		create_game_failed(const char* detail) : Exception("Failed to create game.", detail) {}
	};

	class invalid_input : public Exception {
	public:
		invalid_input() = delete;
		invalid_input(const char* detail) : Exception("Invalid Input", detail) {}
	};

	class winapi_error : public Exception {
	public:
		winapi_error() = delete;
		winapi_error(const char* detail) : Exception("WinAPI Error", detail) {}
	};
	
}


#endif