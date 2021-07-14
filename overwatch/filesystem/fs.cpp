#include "fs.hpp"

Filesystem::Filesystem::Filesystem(const pathT& pathFS, const Code& codeChkOrCreate) {
	this->path = path;

	try {
		std::filesystem::exists(path.parent_path());
		if (codeChkOrCreate == Code::CREATE)
			std::filesystem::create_directories(path);
	} catch (const std::filesystem::filesystem_error& e) {
		throw e;
	}
}

const Filesystem::pathT& Filesystem::Filesystem::getPath() const noexcept{
	return this->path;
}

const Filesystem::listPathT Filesystem::Filesystem::iteratePath() const noexcept{
	listPathT rtn;
	for (auto it : std::filesystem::directory_iterator(this->path))
		rtn.push_back(it.path());
	return rtn;
}

const Filesystem::Code& Filesystem::Filesystem::getRequestType() const noexcept {
	return this->requestCode;
}