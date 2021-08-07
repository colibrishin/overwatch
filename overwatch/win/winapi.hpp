#ifndef PROCESS_H
#define PROCESS_H

#include <Windows.h>
#include "psapi.h"
#include "tlhelp32.h"
#include "process.h"

#include <filesystem>
#include <map>
#include <string>
#include <unordered_set>

#include "../macros/global.hpp"

namespace WinAPI {
	namespace Process {

		namespace Types {
			using mapProcessList = std::map<unsigned long long, STRING>;
		}

		namespace Constants {
			constexpr unsigned short MAX_WINDOWS_PATH_LENGTH = 32767;
		}

		void getProcesses(WinAPI::Process::Types::mapProcessList& plist);
		void getPathProcess(std::filesystem::path& pathProc, const unsigned int& nPID);
		// TODO: Refactor to Exception
	}

	namespace Hotkey {
		namespace Constants {
			constexpr const unsigned char KEYNAME_CHAR_LENGTH = 128;
			constexpr const unsigned int NOREPEAT = 0x4000;

			static unsigned int VKCurrentHotkey = 0x0004;
			static unsigned int uint32CurrentCmbntn = 0x4001;
			static bool isKeySet = true;
		}
	}
}


#endif