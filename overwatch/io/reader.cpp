#include "../io/reader.hpp"

Reader::Serializer::Serializer(const Filesystem::pathT& pathTarget, const std::streamsize& nPos, const Code& requestCode) {
	this->path = pathTarget;
	this->currPos = nPos;
	this->chIO.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
	try {
		switch (requestCode) {
		case Code::READ:
			this->openRFile();
			break;
		case Code::WRITE:
			this->openWFile();
			break;
			/*
			case Code::APPEND:
				this->openWAFile();
				break;
			*/
		}

		if (requestCode == Code::READ)
			seekRFile();
		else if (requestCode == Code::WRITE /* || requestCode == Code::APPEND */)
			seekWFile();
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

void Reader::Serializer::readFile(char* buf) {
	try {
		this->chIO.seekg(this->currPos);
		this->chIO.read(buf, sizeof(buf));
	}
	catch (const std::ios::failure& e) {
		throw e;
	}
}

void Reader::Serializer::writeFile(char* buf, const std::streamsize& _BufSize) {
	try {
		this->chIO.seekp(this->currPos);
		this->chIO.write(buf, sizeof(buf));
	}
	catch (const std::ios::failure& e) {
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
		this->chIO.seekg(this->currPos);
	} catch (std::ios::failure& e){
		throw e;
	}
}

void Reader::Serializer::seekWFile(){
	try {
		this->chIO.seekp(this->currPos);
	} catch (std::ios::failure& e){
		throw e;
	}
}