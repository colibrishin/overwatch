#include "fs.hpp"

Filesystem::Filesystem::Filesystem() {
	this->requestCode = Code::INIT;
}

Filesystem::Filesystem::Filesystem(const pathT& pathFS, const Code& codeChkOrCreate) {
	this->path = pathFS;
	this->requestCode = codeChkOrCreate;

	try {
		if (codeChkOrCreate == Code::CHECK)
			std::filesystem::exists(pathWithoutFilename());
		if (codeChkOrCreate == Code::CREATE)
			std::filesystem::create_directories(pathWithoutFilename());
	} catch (const std::filesystem::filesystem_error& e) {
		throw e;
	}
}

Filesystem::pathT Filesystem::Filesystem::pathWithoutFilename() const noexcept{
	return this->path.has_filename() ? this->path.parent_path() : this->path;
}

const Filesystem::pathT& Filesystem::Filesystem::getPath() const noexcept{
	return this->path;
}

void Filesystem::Filesystem::move(const pathT& newPath) {
	try {
		std::filesystem::rename(this->path, newPath);
		this->path = newPath;
	}
	catch (const std::filesystem::filesystem_error& e) {
		throw e;
	}
}

void Filesystem::Filesystem::copy(const pathT& ToPath) {
	try {
		std::filesystem::exists(this->path);
		if (std::filesystem::is_regular_file(this->path))
			std::filesystem::copy_file(this->path, ToPath);
		else if (std::filesystem::is_directory(this->path))
			std::filesystem::copy(this->path, ToPath);
	}
	catch (const std::filesystem::filesystem_error& e) {
		throw e;
	}
}

void Filesystem::Filesystem::remove() {
	try {
		std::filesystem::remove(this->path);
	}
	catch (const std::filesystem::filesystem_error& e) {
		throw e;
	}
}

const Filesystem::listPathT Filesystem::Filesystem::iteratePath() const noexcept{
	listPathT rtn;
	for (auto& it : std::filesystem::directory_iterator(pathWithoutFilename()))
		rtn.push_back(it.path());
	return rtn;
}

const Filesystem::Code& Filesystem::Filesystem::getRequestType() const noexcept {
	return this->requestCode;
}