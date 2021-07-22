#include <algorithm>
#include "utility.hpp"
#include "../exception.hpp"

void Utility::_String::_charNULLSafeCopy(char* DST, unsigned long long DSTSize, const char* SRC, unsigned long long SRCSize) {
	bool isOverflow;
	if (isOverflow = (SRCSize > DSTSize)) SRCSize = DSTSize;

	std::streamsize size = DSTSize > SRCSize ? SRCSize : DSTSize;

	for (int i = 0; i < size; ++i) {
		DST[i] = SRC[i];
	}

	if (isOverflow) DST[DSTSize - 1] = '\0';
	else DST[size - 1] = '\0';
}

void Utility::_String::_wcharNULLSafeCopy(wchar_t* DST, unsigned long long DSTSize, const wchar_t* SRC, unsigned long long SRCSize) {
	bool isOverflow;
	if (isOverflow = (SRCSize > DSTSize)) SRCSize = DSTSize;

	std::streamsize size = DSTSize > SRCSize ? SRCSize : DSTSize;

	for (int i = 0; i < size; ++i) {
		DST[i] = SRC[i];
	}

	if (isOverflow) DST[DSTSize - 1] = L'\0';
	else DST[size - 1] = '\0';
}

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
	for (int i = 0; i < SZ; ++i) {
		if (SRC[i] == L'\0') {
			DST[i] = L'\0';
			break;
		}
		DST[i] = SRC[i];
	}
	DST[SZ - 1] = L'\0';
}

const std::time_t Utility::getUnixTime() noexcept {
	return std::time(nullptr);
}

bool Utility::_String::_isNumber(const std::string& s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool Utility::_String::_isWNumber(const std::wstring& s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::iswdigit);
}

bool Utility::_String::_isAlphabetic(const std::string& s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isalpha);
}

bool Utility::_String::_isWAlphabetic(const std::wstring& s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isalpha);
}

bool Utility::_String::_isStrEmpty(const char* s) {
	return s[0] == '\0';
}
bool Utility::_String::_isWStrEmpty(const wchar_t* s) {
	return s[0] == L'\0';
}