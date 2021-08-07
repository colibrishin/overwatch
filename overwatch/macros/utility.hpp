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
		std::wstring _charToWchar(const char* target);
		bool _isNumber(const std::string& s);
		bool _isWNumber(const std::wstring& s);
		bool _isAlphabetic(const std::string& s);
		bool _isWAlphabetic(const std::wstring& s);
		bool _isStrEmpty(const char* s);
		bool _isWStrEmpty(const wchar_t* s);
		bool _isFileNameAvailableWS(const std::wstring& s);
		bool _isFileNameAvailableS(const std::string& s);
	}

	const std::time_t getUnixTime() noexcept;
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
#define ISALPHA(x) Utility::_String::_isWAlphabetic(x)
#define ISFILENAMEAVAILABLE(x) Utility::_String::_isFileNameAvailableWS(x)
#define CHAR_TO_WSTR(x) Utility::_String::_charToWchar(x)
#else
#define MEMNULLCPY_S(x,y,z,w) _MEMNULLCPY_S(x,y,z,w)
#define MEMNULLCPY(x, y, z) _MEMNULLCPY(x,y,z)
#define ISNUMBER(x) Utility::_String::isNumber(x)
#define STREMPTY(x) Utility::_String::_isStrEmpty(x)
#define ISALPHA(x) Utility::_String::_isAlphabetic(x)
#define ISFILENAMEAVAILABLE(x) Utility::_String::_isFileNameAvailableS(x)
#define CHAR_TO_WSTR(x) x
#endif

#endif
