#ifndef UNICDOE_H
#define UNICODE_H

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif

#ifdef UNICODE
#define ENCODING_TYPE wchar_t
#define STRING std::wstring
#define CIN std::wcin
#define COUT std::wcout
#define UtoA(x) UtilityString::convertToChar(x)
#else
#define ENCODING_TYPE char
#define STRING std::string
#define CIN std::cin
#define COUT std::cout
#define UtoA(x) x
#endif

#endif