#ifndef LOADER_H
#define LOADER_H

#include <exception>
#include <cstdarg>
#include <string>

#include "../data/data.hpp"
#include "../data/models.hpp"
#include "../exception_class.hpp"
#include "../filesystem/fs.hpp"
#include "../io/reader.hpp"

namespace Loader {
	namespace Exceptions {
		class programming_error : public ErrorStrException {
		public:
			programming_error() = delete;
			programming_error(const char* detail) : ErrorStrException("Programming Error", detail) {}
		};

		class file_invalidate : public ErrorStrException {
		public:
			file_invalidate() = delete;
			file_invalidate(const char* detail) : ErrorStrException("File might have been corrupted.", detail) {}
		};
	}

	enum class Code {
		OPEN = 1,
		NEW = 2,
		WRITE = 3
	};

	template<class T>
	class Loader : public ExceptionClass { 
		Data::Form::Hashable_Data<T> data;
		Filesystem::Filesystem fs;
		Reader::Serializer io;
		Code request;

	public:
		Loader() = delete;
		Loader(const Filesystem::pathT& _PathToFile, const Code& _RequestCode) : fs(_PathToFile, Filesystem::Code::CHECK), io(_PathToFile, 0, Reader::Code::READ) {

			if (_RequestCode == Code::NEW) {
				this->fs = Filesystem::Filesystem(_PathToFile, Filesystem::Code::CREATE);
				this->io = Reader::Serializer(_PathToFile, 0, Reader::Code::WRITE);
			} 
			else if (_RequestCode == Code::WRITE) {
				this->fs = Filesystem::Filesystem(_PathToFile, Filesystem::Code::CHECK);
				this->io = Reader::Serializer(_PathToFile, 0, Reader::Code::APPEND);
			}

			bool isCatch = false;

			if (isCatch = fs.good()) this->pException = fs.getException();
			else if (isCatch = io.good()) this->pException = io.getException();
			if (isCatch) return;

			if (!data.validateValue()) {
				throw Exceptions::file_invalidate("Hash mismatch");
				this->pException = std::current_exception();
				return;
			}

			if(_RequestCode == Code::OPEN) io.readFile(reinterpret_cast<char*>(&data));
		}
		Loader(const Loader& _ref) = delete;
		Loader& operator=(const Loader& _ref) = delete;
	};
}

#endif