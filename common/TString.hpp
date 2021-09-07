#pragma once
#include <tchar.h>
#include <windows.h>
#include <string>

//
// A TString can be either multiple-bytes string or wide-char string.
// When multiple-bytes string, the code page is CP_ACP.
// When wide-char string, it is UTF-16-LE encoded string.
//
#ifdef _UNICODE
using TString = std::wstring;
#else
using TString = std::string;
#endif

//
// For a multiple-bytes string, the default code-page is CP_ACP.
// Of course, you can specify code-page to override it.
//
TString TStringBuilder(PCSTR LpMultiByteStr, UINT CodePage = CP_ACP);

//
// For a wide-char string, it must be UTF-16-LE encoded.
// So `CodePage` is not needed.
//
TString TStringBuilder(PCWSTR LpWideCharStr);

//
// For a multiple-bytes string, the default code-page is CP_ACP.
// Of course, you can specify code-page to override it.
//
TString TStringBuilder(const std::string& RefMultiByteStr, UINT CodePage = CP_ACP);

//
// For a wide-char string, it must be UTF-16-LE encoded.
// So `CodePage` is not needed.
//
TString TStringBuilder(const std::wstring& RefWideCharStr);

//
// Convert a TString to a multiple-bytes string with specified code-page.
//
std::string TStringEncode(PTSTR LpTString, UINT CodePage);
std::string TStringEncode(const TString& RefTStr, UINT CodePage);

//
// Convert a multiple-bytes string to a TString with specified code-page.
//
TString TStringDecode(PCSTR LpMultiByteStr, UINT CodePage = CP_ACP);
TString TStringDecode(const std::string& RefMultiByteStr, UINT CodePage = CP_ACP);

//
// Format function for TString
//
template<typename... __Ts>
TString TStringFormat(const TString& Format, __Ts&&... Args) {
    TString s;
    s.resize(_sctprintf(Format.c_str(), std::forward<__Ts>(Args)...) + 1);
    s.resize(_sntprintf_s(s.data(), s.capacity(), _TRUNCATE, Format.c_str(), std::forward<__Ts>(Args)...));
    return s;
}

