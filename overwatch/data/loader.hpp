#ifndef LOADER_H
#define LOADER_H

#include <exception>
#include <cstdarg>
#include <string>

#include "../hash/hash.hpp"
#include "../data/data.hpp"
#include "../data/models.hpp"
#include "../exception.hpp"
#include "../filesystem/fs.hpp"
#include "../io/reader.hpp"

namespace Loader {
	enum class Code {
		NEW,
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
		Loader(const Filesystem::pathT& _PathToFile, const Code& _RequestCode) {
			
			this->requestCode = _RequestCode;
			try {
				if (_RequestCode == Code::NEW) {
					this->fs = Filesystem::Filesystem(_PathToFile, Filesystem::Code::CREATE);
					this->io = Reader::Serializer(_PathToFile, 0, Reader::Code::WRITE);
				}

				if (_RequestCode == Code::OPEN) {
					this->fs = Filesystem::Filesystem(_PathToFile, Filesystem::Code::CHECK);
					this->io = Reader::Serializer(_PathToFile, 0, Reader::Code::READ);
				}

				if (_RequestCode == Code::WRITE) {
					this->fs = Filesystem::Filesystem(_PathToFile, Filesystem::Code::CREATE);
					this->io = Reader::Serializer(_PathToFile, 0, Reader::Code::APPEND);
				}

				if (_RequestCode == Code::OPEN) {
					io.readFile(reinterpret_cast<char*>(&data), sizeof(data));
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

		const Data::Form::Hashable_Data<T>& getData() const noexcept { return this->data; }
		void setData(const Data::Form::Hashable_Data<T>& _NewData) noexcept { this->data = _NewData; }
		Hash::SHA256 getHash() const noexcept { return this->data.getHash(); }
		void writeData() {
			Data::Form::Hashable_Data<T>* ptr = const_cast<Data::Form::Hashable_Data<T>*>(&this->data);
			try {
				this->io.writeFile(reinterpret_cast<char*>(ptr), sizeof(this->data));
			}
			catch (const std::ios::failure& e) {
				throw Exceptions::Exception(e);
			}
		}
		void renameData(const Filesystem::pathT& newPath) {
			try {
				this->fs.move(newPath);
				this->io.changeFile(newPath);
			}
			catch (const std::filesystem::filesystem_error& e) {
				throw Exceptions::Exception(e);
			}
			catch (const std::ios::failure& e) {
				throw Exceptions::Exception(e);
			}
		}

	};
}

#endif