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
	enum class Code {
		OPEN = 1,
		NEW = 2,
		WRITE = 3
	};

	template<class T>
	class Loader : public Exceptions::ExceptionClass { 
		Data::Form::Hashable_Data<T> data;
		Filesystem::Filesystem fs;
		Reader::Serializer io;
		Code requestCode;

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

			if (!(isCatch = fs.good())) this->pException = fs.getException();
			else if (!(isCatch = io.good())) this->pException = io.getException();
			if (!isCatch) return;

			if (_RequestCode == Code::OPEN) {
				try {
					io.readFile(reinterpret_cast<char*>(&data));
				}
				catch (std::logic_error& e) {
					throw Exceptions::LoaderException::programming_error(e.what());
					this->pException = std::current_exception();
				}
				catch (std::runtime_error& e) {
					throw Exceptions::LoaderException::file_invalidate(e.what());
					this->pException = std::current_exception();
				}
				if (!data.validateValue()) {
					throw Exceptions::LoaderException::file_invalidate("Hash mismatch");
					this->pException = std::current_exception();
					return;
				}
			}
		}
		Loader(const Loader& _ref) = delete;
		Loader& operator=(const Loader& _ref) = delete;

		const T& getData() const noexcept { return this->data.getValue(); }
		void setData(const T& _NewData) noexcept { this->data.setValue(_NewData); }
		void writeData() {
			if (this->requestCode != Code::WRITE || this->requestCode != Code::NEW) {
				throw Exceptions::LoaderException::programming_error("writeData() didn't got a WRITE nor NEW defined Loader.");
				this->pException = std::current_exception();
				return;
			}
		}

	};
}

#endif