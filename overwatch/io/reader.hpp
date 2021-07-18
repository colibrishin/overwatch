#ifndef READER_H
#define READER_H

#include <filesystem>
#include <fstream>

#include "../macros/global.hpp"
#include "../filesystem/fs.hpp"

namespace Reader {
	enum class Code {
		INIT,
		READ,
		WRITE,
		APPEND
	};

	class Serializer {
	private:
		Filesystem::pathT path;
		std::streamsize sizeFile;
		std::streamsize currPos;
		std::streamsize step;
		std::fstream chIO;

		Code requestCode;

		void openRFile();
		void openWFile();
		void openAFile();
		void seekRFile();
		void seekWFile();
	public:
		Serializer();
		Serializer(const Filesystem::pathT& _TargetFile, const std::streamsize& _PosToSeek, const Code& _ReadOrWrite);
		Serializer(const Filesystem::pathT& _TargetFile, const std::streamsize& _PosToSeek, const std::streamsize& _SeekStep, const Code& _ReadOrWrite);
		void nextPos() noexcept;
		void setStep(const std::streamsize& _SeekStep) noexcept;
		void readFile(char* buf, const std::streamsize _BufSize);
		void writeFile(char* buf, const std::streamsize& _BufSize);
		void changeFile(const Filesystem::pathT& newPath);
		void release();
		std::streamsize getSizeFile() const noexcept;
	};
}
#endif