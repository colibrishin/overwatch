#ifndef UTILITY_H
#define UTILITY_H

#include <cstring>
#include <string>
#include <ios>

#define _wmemset(x,y,z) std::wmemset(x, y, z / sizeof(wchar_t));
#define _wmemcpy(x,y,z) std::wmemcpy(x, y, z / sizeof(wchar_t));
#define _wwcsncmp(x,y,z) std::wcsncmp(x, y, z / sizeof(wchar_t));
#define NULL_CHAR '\0'

#ifdef UNICODE
#define UT_memset(x,y,z) _wmemset(x,y,z)
#define UT_memcpy(x,y,z) _wmemcpy(x,y,z)
#define UT_strempty(x) UtilityString::iswStrEmpty(x)
#define NULL_CHAR L'\0'
#else
#define UT_memset(x,y,z) std::memcpy(x,y,z)
#define UT_memcpy(x,y,z) std::memset(x,y,z)
#define UT_strempty(x) UtilityString::isStrEmpty(x)
#define NULL_CHAR '\0'
#endif

namespace UtilityString {
	void _charNULLbreakCopy(char* DST, const char* SRC, unsigned long long SZ);
	void _wcharNULLbreakCopy(wchar_t* DST, const wchar_t* SRC, unsigned long long SZ);
	bool isNumber(const std::string& s);
	bool isWNumber(const std::wstring& s);
	bool isStrEmpty(const char* s);
	bool iswStrEmpty(const wchar_t* s);
}

#ifdef UNICODE
#define UT_NULLbreakCopy(x, y, z) UtilityString::_wcharNULLbreakCopy(x, y, z)
#define UT_isNumber(x) UtilityString::isWNumber(x)
#else
#define UT_NULLbreakCopy(x, y, z) UtilityString::_charNULLbreakCopy(x, y, z)
#define UT_isNumber(x) UtilityString::isNumber(x)
#endif

#endif