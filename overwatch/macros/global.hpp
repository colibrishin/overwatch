#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <iostream>

#ifdef _UNICODE
#define UNICODE 1
#endif

#define _WMEMSET(x,y,z) std::wmemset(x, y, z / sizeof(wchar_t))
#define _WMEMCPY(x,y,z) std::wmemcpy(x, y, z / sizeof(wchar_t))

#ifdef UNICODE
#define CHAR wchar_t
#define STRING std::wstring
#define CIN std::wcin
#define NULL_CHAR L'\0'
#define MEMCPY(x,y,z) _WMEMCPY(x,y,z)
#define MEMSET(x,y,z) _WMEMSET(x,y,z)
#else
#define CHAR char
#define STRING std::string
#define STRING std::cin
#define NULL_CHAR '\0'
#define MEMCPY(x,y,z) std::memcpy(x,y,z)
#define MEMSET(x,y,z) std::memset(x,y,z)
#endif

constexpr unsigned long long MAX_WINDOWS_CHAR = 32767;
constexpr unsigned long long MAX_DATA_NAME = 500 + 1;

using strNameT = CHAR[MAX_DATA_NAME];
using strProcT = CHAR[MAX_WINDOWS_CHAR + 1];
using strPathT = CHAR[MAX_WINDOWS_CHAR + 1];
using binStatusT = unsigned short;

#endif