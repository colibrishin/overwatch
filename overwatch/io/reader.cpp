#include "../io/reader.hpp"

Reader::Serializer::Serializer() {
	this->requestCode = Code::INIT;
}

Reader::Serializer::Serializer(const Filesystem::pathT& pathTarget, const std::streamsize& nPos, const Code& requestCode) {
	this->path = pathTarget;
	this->currPos = nPos;
	this->sizeFile = 0;
	this->step = 0;
	this->requestCode = requestCode;
	this->chIO.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
	try {
		switch (requestCode) {
		case Code::READ:
			this->openRFile();
			break;
		case Code::WRITE:
			this->openWFile();
			break;
		case Code::APPEND:
			this->openAFile();
			break;
		}

		if (requestCode == Code::READ)
			seekRFile();
		else if (requestCode == Code::APPEND)
			seekWFile();

		this->release();
	}
	catch (const std::ios::failure& e) {
		throw e;
	}
}

Reader::Serializer::Serializer(const Filesystem::pathT& pathTarget, const std::streamsize& nPos, const std::streamsize& nStep, const Code& requestCode) {
	try {
		this->Serializer::Serializer(pathTarget, nPos, requestCode);
		this->step = nStep;
	}
	catch (const std::ios::failure& e) {
		throw e;
	}
}

void Reader::Serializer::nextPos() noexcept{
	this->currPos += this->step;
}

void Reader::Serializer::setStep(const std::streamsize& nStep) noexcept {
	this->step = nStep;
}

void Reader::Serializer::readFile(char* buf, const std::streamsize _BufSize) {
	try {
		this->openRFile();
		this->chIO.seekg(this->currPos);
		this->chIO.read(buf, _BufSize);
		this->release();
	}
	catch (const std::ios::failure& e) {
		throw e;
	}
}

void Reader::Serializer::writeFile(char* buf, const std::streamsize& _BufSize) {
	try {
		this->openWFile();
		this->chIO.seekp(this->currPos);
		this->chIO.write(buf, _BufSize);
		this->sizeFile = _BufSize;
		this->release();
	}
	catch (const std::ios::failure& e) {
		throw e;
	}
}

void Reader::Serializer::openAFile() {
	if (this->chIO.is_open()) this->chIO.close();
	try {
		this->chIO.open(this->path, std::ios::app | std::ios::binary);
	}
	catch (std::ios::failure& e) {
		throw e;
	}
}

void Reader::Serializer::openWFile() {
	if(this->chIO.is_open()) this->chIO.close();
	try {
		this->chIO.open(this->path, std::ios::out | std::ios::binary);
	} catch (std::ios::failure& e){
		throw e;
	}
}

void Reader::Serializer::openRFile(){
	if(this->chIO.is_open()) this->chIO.close();
	try {
		this->chIO.open(this->path, std::ios::in | std::ios::binary);
	} catch (std::ios::failure& e){
		throw e;
	}
}

void Reader::Serializer::seekRFile(){
	try {
		this->chIO.seekg(std::ios::end);
		this->sizeFile = this->chIO.tellg();
		this->chIO.seekg(this->currPos);
	} catch (std::ios::failure& e){
		throw e;
	}
}

void Reader::Serializer::seekWFile(){
	try {
		this->chIO.seekp(std::ios::end);
		this->sizeFile = this->chIO.tellp();
		this->chIO.seekp(this->currPos);
	} catch (std::ios::failure& e){
		throw e;
	}
}

void Reader::Serializer::changeFile(const Filesystem::pathT& newPath) {
	try {
		if (this->requestCode == Code::WRITE)
			this->requestCode = Code::APPEND;

		this->Serializer::Serializer(newPath, this->currPos, this->step, this->requestCode);
	}
	catch (std::ios::failure& e) {
		throw e;
	}
}

std::streamsize Reader::Serializer::getSizeFile() const noexcept {
	return this->sizeFile;
}

void Reader::Serializer::release() {
	try {
		this->chIO.close();
	}
	catch (std::ios::failure& e) {
		throw e;
	}
}