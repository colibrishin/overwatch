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

#include "global.hpp"

namespace WinAPI {
	namespace Process {

		namespace Types {
			using mapProcessList = std::map<unsigned int, STRING>;
		}

		namespace Constants {
			constexpr unsigned short MAX_WINDOWS_PATH_LENGTH = 32767;
			// There's same macro in winapi.hpp

			constexpr const char* STR_ERR_HANDLE_ACQUIRE_FAILED = "WinAPI Failed to acquire Process handle";
			constexpr const char* STR_ERR_PROCESS32FIRSTERROR = "WinAPI Failed to acquire Process32First handle";
			constexpr const char* STR_ERR_HPROCESS_NULL = "WinAPI Failed to acquire HProcess handle";
			constexpr const char* STR_ERR_QUERY_PROCESS_IMAGE_FAILED = "WinAPI Process Querying Failed";


			namespace Returns {
				constexpr const unsigned short HANDLE_ACQUIRE_FAILED = 2;
				constexpr const unsigned short PROCESS32FIRSTERROR = 4;
				constexpr const unsigned short HPROCESS_NULL = 8;
				constexpr const unsigned short QUERY_PROCESS_IMAGE_FAILED = 16;
			}
		}

		namespace Utility {
			std::string getErrorMessage(unsigned short code);
		}
		// TODO: Refactor to Exception
		
		Process::Enums::ReturnCode getProcesses(Process::Types::mapProcessList&);
		Process::Enums::ReturnCode getPathProcess(std::filesystem::path&, const unsigned int&);
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