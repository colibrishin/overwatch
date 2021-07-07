#include "fs.hpp"
#include <exception>

Filesystem::Filesystem::Filesystem(const pathT& pathFS, const Code& codeChkOrCreate) {
	this->path = path;

	try {
		std::filesystem::exists(path.parent_path());
		if (codeChkOrCreate == Code::CREATE)
			std::filesystem::create_directories(path);
		this->pException = nullptr;
	} catch (const std::filesystem::filesystem_error& e) {
		this->pException = std::current_exception();
	}
}

Filesystem::pathT Filesystem::Filesystem::getPath() const noexcept{
	return this->path;
}

Filesystem::Code Filesystem::Filesystem::getRequestType() const noexcept {
	return this->requestCode;
}