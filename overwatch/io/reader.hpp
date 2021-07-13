#ifndef READER_H
#define READER_H

#include <filesystem>
#include <fstream>

#include "../macros/global.hpp"
#include "../filesystem/fs.hpp"
#include "../exception_class.hpp"

namespace Reader {
	enum class Code {
		READ = 1,
		WRITE = 2,
		APPEND = 3
	};

	class Serializer : public Exceptions::ExceptionClass{
	private:
		Filesystem::pathT path;
		std::streamsize currPos;
		std::streamsize step;
		std::fstream chIO;

		bool isLastRequestOK;
		Code requestCode;

		bool checkChannelSanity() const noexcept;
		void openRFile() noexcept;
		void openWFile() noexcept;
		void openWAFile() noexcept;
		void seekRFile() noexcept;
		void seekWFile() noexcept;
	public:
		Serializer(const Filesystem::pathT& _TargetFile, const std::streamsize& _PosToSeek, const Code& _ReadOrWrite);
		Serializer(const Filesystem::pathT& _TargetFile, const std::streamsize& _PosToSeek, const std::streamsize& _SeekStep, const Code& _ReadOrWrite);
		void nextPos() noexcept;
		void setStep(const std::streamsize& _SeekStep) noexcept;
		void readFile(char* buf);
		void writeFile(char* buf, const std::streamsize& _BufSize);
		bool good() const noexcept;
	};
}
#endif