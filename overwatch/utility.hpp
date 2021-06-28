#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "global.hpp"

namespace Utility {
	namespace _String {
		void _charNULLbreakCopy(char* DST, const char* SRC, unsigned long long SZ);
		void _wcharNULLbreakCopy(wchar_t* DST, const wchar_t* SRC, unsigned long long SZ);
		bool _isNumber(const std::string& s);
		bool _isWNumber(const std::wstring& s);
		bool _isStrEmpty(const char* s);
		bool _isWStrEmpty(const wchar_t* s);
	}
}

#ifdef UNICODE
#define MEMNULLCPY(x, y, z) Utility::_String::_wcharNULLbreakCopy(x, y, z)
#define ISNUMBER(x) Utility::_String::_isWNumber(x)
#define STREMPTY(x) Utility::_String::_isWStrEmpty(x)
#else
#define MEMNULLCPY(x, y, z) Utility::_String::_charNULLbreakCopy(x, y, z)
#define ISNUMBER(x) Utility::_String::isNumber(x)
#define STREMPTY(x) Utility::_String::_isStrEmpty(x)
#endif

#endif
