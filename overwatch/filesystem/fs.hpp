#ifndef FS_H
#define FS_H

#include <filesystem>

#include "../macros/global.hpp"
#include "../exception_class.hpp"

namespace Filesystem {
	using pathT = std::filesystem::path;

	enum class Code {
		CHECK = 1,
		CREATE = 2
	};

	class Filesystem : public Exceptions::ExceptionClass {
		pathT path;
		Code requestCode;

	public:
		Filesystem() = delete;
		Filesystem(const pathT& _Path, const Code& _RequestCode);
		pathT getPath() const noexcept;
		Code getRequestType() const noexcept;
	};
}

#endif