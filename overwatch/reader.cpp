#include "reader.hpp"

Reader::Serializer::Serializer(const Filesystem::pathT& pathTarget, const std::streamsize& nPos, const Code& requestCode) {
	this->path = pathTarget;
	this->currPos = nPos;
	this->chIO.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
	this->pException = nullptr;
	this->isLastRequestOK = true;

	switch(requestCode){
		case Code::READ:
			this->openRFile();
			break;
		case Code::WRITE:
			this->openWFile();
			break;
		case Code::APPEND:
			this->openWAFile();
			break;
		default:
			throw std::runtime_error("Unknown Error, Occurred at Serializer() constructor.");
			this->pException = std::current_exception();
			this->isLastRequestOK = false;
			break;
	}
	if (!(isLastRequestOK = checkChannelSanity())) return;

	if (requestCode == Code::READ) 
		seekRFile();
	else if (requestCode == Code::WRITE || requestCode == Code::APPEND)
		seekWFile();
	isLastRequestOK = checkChannelSanity();
}

Reader::Serializer::Serializer(const Filesystem::pathT& pathTarget, const std::streamsize& nPos, const std::streamsize& nStep, const Code& requestCode) {
	this->Serializer::Serializer(pathTarget, nPos, requestCode);
	this->step = nStep;
}

void Reader::Serializer::nextPos() noexcept{
	this->currPos += this->step;
}

void Reader::Serializer::setStep(const std::streamsize& nStep) noexcept {
	this->step = nStep;
}

void Reader::Serializer::readFile(char* buf) {
	if (!this->chIO.is_open()){
		throw std::logic_error("File is not opened.");
		this->isLastRequestOK = false;
		return;
	}
	this->chIO.seekg(std::ios::end);
	if ((this->chIO.tellp() - this->currPos) < 0) {
		throw std::runtime_error("File is too short to read.");
		buf = new char('\n');
		return;
	}
	this->chIO.seekg(this->currPos);
	this->chIO.read(buf, sizeof(buf));
	this->isLastRequestOK = true;
}

void Reader::Serializer::writeFile(char* buf, const std::streamsize& _BufSize) {
	if (!this->chIO.is_open()) {
		throw std::logic_error("File is not opened.");
		this->isLastRequestOK = false;
		return;
	}
	this->chIO.seekp(this->currPos);
	this->chIO.write(buf, sizeof(buf));
	this->isLastRequestOK = true;
}

void Reader::Serializer::openWFile() noexcept{
	if(this->chIO.is_open()) this->chIO.close();
	try {
		this->chIO.open(this->path, std::ios::in | std::ios::binary);
		this->isLastRequestOK = true;
	} catch (std::ios::failure& e){
		this->pException = std::current_exception();
		this->isLastRequestOK = false;
	}
}

void Reader::Serializer::openWAFile() noexcept{
	if(this->chIO.is_open()) this->chIO.close();
	try {
		this->chIO.open(this->path, std::ios::in | std::ios::binary);
		this->isLastRequestOK = true;
	} catch (std::ios::failure& e){
		this->pException = std::current_exception();
		this->isLastRequestOK = false;
	}
}

void Reader::Serializer::openRFile() noexcept{
	if(this->chIO.is_open()) this->chIO.close();
	try {
		this->chIO.open(this->path, std::ios::in | std::ios::binary);
		this->isLastRequestOK = true;
	} catch (std::ios::failure& e){
		this->pException = std::current_exception();
		this->isLastRequestOK = false;
	}
}

void Reader::Serializer::seekRFile() noexcept{
	try {
		this->chIO.seekg(this->currPos);
		this->isLastRequestOK = true;
	} catch (std::ios::failure& e){
		this->pException = std::current_exception();
		this->isLastRequestOK = false;
	}
}

void Reader::Serializer::seekWFile() noexcept{
	try {
		this->chIO.seekp(this->currPos);
		this->isLastRequestOK = true;
	} catch (std::ios::failure& e){
		this->pException = std::current_exception();
		this->isLastRequestOK = false;
	}
}

bool Reader::Serializer::checkChannelSanity() const noexcept{
	return (this->pException != nullptr);
}

bool Reader::Serializer::good() const noexcept {
	return checkChannelSanity() && this->isLastRequestOK;
}