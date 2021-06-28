#include "utility.hpp"
#include <algorithm>
#include <cmath>

void UtilityString::_charNULLbreakCopy(char* DST, const char* SRC, unsigned long long SZ) {
	for (int i = 0; i < SZ; ++i) {
		if (SRC[i] == '\0') {
			DST[i] = '\0';
			break;
		}
		DST[i] = SRC[i];
	}
	DST[SZ - 1] = '\0';
}

void UtilityString::_wcharNULLbreakCopy(wchar_t* DST, const wchar_t* SRC, unsigned long long SZ) {
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

bool UtilityString::isNumber(const std::string& s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool UtilityString::isWNumber(const std::wstring& s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::iswdigit);
}

bool UtilityString::isStrEmpty(const char* s) {
	return s[0] == '\0';
}
bool UtilityString::iswStrEmpty(const wchar_t* s) {
	return s[0] == L'\0';
}