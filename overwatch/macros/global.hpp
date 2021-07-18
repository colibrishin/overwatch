#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <iostream>

#ifdef _UNICODE
#define UNICODE 1
#endif

#define _WMEMSET(x,y,z) std::wmemset(x, y, z)
#define _WMEMCPY(x,y,z) std::wmemcpy(x, y, z)
#define _MEMSET(x,y,z) std::memset(x,y,z)
#define _MEMCPY(x,y,z) std::memcpy(x,y,z)

#ifdef UNICODE
#define CHAR wchar_t
#define STRING std::wstring
#define CIN std::wcin
#define NULL_CHAR L'\0'
#define MEMCPY(x,y,z) _WMEMCPY(x,y,z)
#define MEMSET(x,y,z) _WMEMSET(x,y,z)
#define TO_STRING(x) std::to_wstring(x)
#else
#define CHAR char
#define STRING std::string
#define STRING std::cin
#define NULL_CHAR '\0'
#define MEMCPY(x,y,z) _MEMCPY(x,y,z)
#define MEMSET(x,y,z) _MEMSET(x,y,z)
#define TO_STRING(x) std::to_string(x)

#endif

constexpr unsigned long long MAX_WINDOWS_CHAR = 32767;
constexpr unsigned long long MAX_DATA_NAME = 500;

using strNameT = CHAR[MAX_DATA_NAME + 1];
using strProcT = CHAR[MAX_WINDOWS_CHAR + 1];
using strPathT = CHAR[MAX_WINDOWS_CHAR + 1];

#define GAME_DIRECTORY "game/"
#define SNAPSHOT_DIRECTORY "snapshot/"
#define SNAPSHOT_COPY_DIRECTORY "snapshot/copy/"

#define CONFIG_FILE "config.cfg"
#define GAME_FILE_EXTENSION ".gdat"
#define SNAPSHOT_FILE_EXTENSION ".sdat"
#define TMP_FILE_EXTENSION ".tmp"

#endif