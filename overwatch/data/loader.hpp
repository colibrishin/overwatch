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
#include "../data/models.hpp"

namespace Loader {
	enum class Code {
		NEW,
		OPEN,
		WRITE
	};

	template<class T>
	class Loader { 
		Models::Model<T> data;
		Filesystem::Filesystem fs;
		Reader::Serializer io;
		Code requestCode;
	private:
		void setLoader(const Filesystem::pathT& _PathToFile, const std::streamsize seekPos, const Code& _RequestCode) {
			this->requestCode = _RequestCode;
			try {
				if (_RequestCode == Code::NEW) {
					this->fs = Filesystem::Filesystem(_PathToFile, Filesystem::Code::CREATE);
					this->io = Reader::Serializer(_PathToFile, seekPos, Reader::Code::WRITE);
				}

				if (_RequestCode == Code::OPEN) {
					this->fs = Filesystem::Filesystem(_PathToFile, Filesystem::Code::CHECK);
					this->io = Reader::Serializer(_PathToFile, seekPos, Reader::Code::READ);
				}

				if (_RequestCode == Code::WRITE) {
					this->fs = Filesystem::Filesystem(_PathToFile, Filesystem::Code::CREATE);
					this->io = Reader::Serializer(_PathToFile, seekPos, Reader::Code::APPEND);
				}

				if (_RequestCode == Code::OPEN) {
					io.readFile(reinterpret_cast<char*>(&data), sizeof(data));
					if (!data.getHashable()->validateValue())
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
	public:
		Loader() = delete;
		Loader(const Filesystem::pathT& _PathToFile, const Code& _RequestCode) {
			try {
				setLoader(_PathToFile, 0, _RequestCode);
			}
			catch (Exceptions::Exception& e) {
				throw e;
			}
		}
		Loader(const Filesystem::pathT& _PathToFile, const std::streamsize seekPos, const Code& _RequestCode) {
			try {
				setLoader(_PathToFile, seekPos, _RequestCode);
			}
			catch (Exceptions::Exception& e) {
				throw e;
			}
		}
		Loader(const Loader& _ref) = delete;
		Loader& operator=(const Loader& _ref) = delete;

		const Filesystem::pathT getPath() const noexcept { return this->fs.getPath(); }
		const Models::Model<T> getModel() { return this->data; }
		const T getData() const noexcept { return this->data.getData(); }
		const std::unique_ptr<Data::Form::Hashable_Data<T>> getHashable() { return this->data.getHashable(); }
		void setData(const Models::Model<T>& _NewData) noexcept { this->data = _NewData; }
		Hash::SHA256 getHash() const noexcept { return this->data.getHashable()->getHash(); }
		void writeData() {
			//std::unique_ptr<Data::Form::Hashable_Data<T>> ptr = const_cast<Data::Form::Hashable_Data<T>*>(&*this->data.getHashable());
			auto ptr = const_cast<T*>(&this->data.getData());
			
			try {
				this->io.writeFile(reinterpret_cast<char*>(ptr), sizeof(this->data.getData()));
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