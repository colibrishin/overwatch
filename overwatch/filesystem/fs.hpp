#ifndef FS_H
#define FS_H

#include <filesystem>
#include <list>
#include <memory>

#include "../macros/global.hpp"

namespace Filesystem {
	using pathT = std::filesystem::path;
	using listPathT = std::list<pathT>;

	enum class Code {
		INIT,
		CHECK,
		CREATE
	};

	class Filesystem {
		pathT path;
		Code requestCode;

	public:
		Filesystem();
		Filesystem(const pathT& _Path, const Code& _RequestCode);

		const pathT& getPath() const noexcept;
		pathT pathWithoutFilename() const noexcept;
		const listPathT iteratePath() const noexcept;
		const Code& getRequestType() const noexcept;
		void move(const pathT& newPath);
		void copy(const pathT& ToPath);
	};
}

#endif