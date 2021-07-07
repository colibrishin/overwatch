#ifndef TYPES_H
#define TYPES_H

#include <filesystem>

namespace Types {
	namespace Constants {
		constexpr const unsigned short INSTANCE_MAX_NAME = 500 + 1;
	}

	using wcharInstanceT = wchar_t[INSTANCE_MAX_NAME];
	using pathT = std::filesystem::path;
}

#endif