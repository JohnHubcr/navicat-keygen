#pragma once
#include "TString.hpp"
#include <vector>
#include <iostream>

#ifdef _UNICODE
#define _tcin wcin
#define _tcout wcout
#define _tcerr wcerr
#else
#define _tcin cin
#define _tcout cout
#define _tcerr cerr
#endif

namespace Helper {

    TString Base64Encode(const std::vector<uint8_t>& Bytes);

    std::vector<uint8_t> Base64Decode(const TString& Base64Str);

    bool ReadInt(int& RefInt, int MinVal, int MaxVal, PCTSTR Prompt, PCTSTR ErrMsg);

}

