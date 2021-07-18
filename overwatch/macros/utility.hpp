#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "../macros/global.hpp"

namespace Utility {
	namespace _String {
		void _charNULLSafeCopy(char* DST, unsigned long long DSTSize, const char* SRC, unsigned long long SRCSize);
		void _wcharNULLSafeCopy(wchar_t* DST, unsigned long long DSTSize, const wchar_t* SRC, unsigned long long SRCSize);
		void _charNULLbreakCopy(char* DST, const char* SRC, unsigned long long SZ);
		void _wcharNULLbreakCopy(wchar_t* DST, const wchar_t* SRC, unsigned long long SZ);
		bool _isNumber(const std::string& s);
		bool _isWNumber(const std::wstring& s);
		bool _isStrEmpty(const char* s);
		bool _isWStrEmpty(const wchar_t* s);
	}

	const std::time_t getUnixTime() { return std::time(nullptr); }
}

#define _WMEMNULLCPY_S(x,y,z,w) Utility::_String::_wcharNULLSafeCopy(x,y,z,w)
#define _MEMNULLCPY_S(x,y,z,w) Utility::_String::_charNULLSafeCopy(x,y,z,w)

#define _WMEMNULLCPY(x,y,z) Utility::_String::_wcharNULLbreakCopy(x, y, z)
#define _MEMNULLCPY(x,y,z) Utility::_String::_charNULLbreakCopy(x, y, z)

#ifdef UNICODE
#define MEMNULLCPY_S(x,y,z,w) _WMEMNULLCPY_S(x,y,z,w)
#define MEMNULLCPY(x, y, z) _WMEMNULLCPY(x,y,z)
#define ISNUMBER(x) Utility::_String::_isWNumber(x)
#define STREMPTY(x) Utility::_String::_isWStrEmpty(x)
#else
#define MEMNULLCPY_S(x,y,z,w) _MEMNULLCPY_S(x,y,z,w)
#define MEMNULLCPY(x, y, z) _MEMNULLCPY(x,y,z)
#define ISNUMBER(x) Utility::_String::isNumber(x)
#define STREMPTY(x) Utility::_String::_isStrEmpty(x)
#endif

#endif
