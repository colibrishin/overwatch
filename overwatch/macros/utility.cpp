#include <algorithm>
#include "utility.hpp"

void Utility::_String::_charNULLbreakCopy(char* DST, const char* SRC, unsigned long long SZ) {
	for (int i = 0; i < SZ; ++i) {
		if (SRC[i] == '\0') {
			DST[i] = '\0';
			break;
		}
		DST[i] = SRC[i];
	}
	DST[SZ - 1] = '\0';
}

void Utility::_String::_wcharNULLbreakCopy(wchar_t* DST, const wchar_t* SRC, unsigned long long SZ) {
	SZ = SZ / sizeof(wchar_t);
	for (int i = 0; i < SZ; ++i) {
		if (SRC[i] == '\0') {
			DST[i] = '\0';
			break;
		}
		DST[i] = SRC[i];
	}
	DST[SZ - 1] = '\0';
}

bool Utility::_String::_isNumber(const std::string& s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool Utility::_String::_isWNumber(const std::wstring& s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::iswdigit);
}

bool Utility::_String::_isStrEmpty(const char* s) {
	return s[0] == '\0';
}
bool Utility::_String::_isWStrEmpty(const wchar_t* s) {
	return s[0] == L'\0';
}