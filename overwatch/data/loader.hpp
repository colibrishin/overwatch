#ifndef LOADER_H
#define LOADER_H

#include <exception>
#include <cstdarg>
#include <string>

#include "../data/data.hpp"
#include "../data/models.hpp"
#include "../exception.hpp"
#include "../filesystem/fs.hpp"
#include "../io/reader.hpp"

namespace Loader {
	enum class Code {
		OPEN,
		WRITE
	};

	template<class T>
	class Loader { 
		Data::Form::Hashable_Data<T> data;
		Filesystem::Filesystem fs;
		Reader::Serializer io;
		Code requestCode;

	public:
		Loader() = delete;
		Loader(const Filesystem::pathT& _PathToFile, const Code& _RequestCode) : fs(_PathToFile, Filesystem::Code::CHECK), io(_PathToFile, 0, Reader::Code::READ) {
			try {
				if (_RequestCode == Code::WRITE) {
					this->fs = Filesystem::Filesystem(_PathToFile, Filesystem::Code::CREATE);
					this->io = Reader::Serializer(_PathToFile, 0, Reader::Code::WRITE);
				}

				if (_RequestCode == Code::OPEN) {
					io.readFile(reinterpret_cast<char*>(&data));
					if (!data.validateValue())
						throw Exceptions::file_invalidate("Hash mismatch");
				}
			}
			catch (const std::ios::failure& e) {
				throw Exceptions::Exception(e);
			}
			catch (const std::filesystem::filesystem_error& e) {
				throw Exceptions::Exception(e);
			}
		}
		Loader(const Loader& _ref) = delete;
		Loader& operator=(const Loader& _ref) = delete;

		const T& getData() const noexcept { return this->data.getValue(); }
		void setData(const T& _NewData) noexcept { this->data.setValue(_NewData); }
		void writeData() {
			T* ptr = const_cast<T*>(&this->getData());
			try {
				this->io.writeFile(reinterpret_cast<char*>(ptr), sizeof(this->data.getValue()));
			}
			catch (const std::ios::failure& e) {
				throw Exceptions::Exception(e);
			}
		}

	};
}

#endif