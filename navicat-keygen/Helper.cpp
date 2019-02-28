#include "Helper.hpp"
#include "ExceptionSystem.hpp"
#include <iostream>

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("Helper.cpp")

namespace Helper {

    TString Base64Encode(const std::vector<uint8_t>& bytes) {
        TString RetValue;
        DWORD pcchString = 0;

        if (bytes.empty())
            return RetValue;

        if (!CryptBinaryToString(bytes.data(),
                                 static_cast<DWORD>(bytes.size()),
                                 CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
                                 NULL,
                                 &pcchString))
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("CryptBinaryToString failed."));

        RetValue.resize(pcchString);

        if (!CryptBinaryToString(bytes.data(),
                                 static_cast<DWORD>(bytes.size()),
                                 CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
                                 RetValue.data(),
                                 &pcchString))
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("CryptBinaryToString failed."));

        while (RetValue.back() == TEXT('\x00'))
            RetValue.pop_back();

        return RetValue;
    }

    std::vector<uint8_t> Base64Decode(const TString& Base64Str) {
        std::vector<uint8_t> RetValue;
        DWORD pcbBinary = 0;

        if (Base64Str.empty())
            return RetValue;

        if (!CryptStringToBinary(Base64Str.c_str(),
                                 NULL,
                                 CRYPT_STRING_BASE64,
                                 NULL,
                                 &pcbBinary,
                                 NULL,
                                 NULL))
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("CryptStringToBinary failed."),
                                  TEXT("Are you sure it is a Base64 string?"));

        RetValue.resize(pcbBinary);

        if (!CryptStringToBinary(Base64Str.c_str(),
                                 NULL,
                                 CRYPT_STRING_BASE64,
                                 RetValue.data(),
                                 &pcbBinary,
                                 NULL,
                                 NULL))
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("CryptStringToBinary failed."));

        return RetValue;
    }

    bool ReadInt(int& RefInt, int MinVal, int MaxVal, PCTSTR Prompt, PCTSTR ErrMsg) {
        int t;
        TString s;
        while (true) {
            _tprintf_s(TEXT("%s"), Prompt);
            if (!std::getline(std::_tcin, s))
                return false;

            if (s.empty())
                continue;

            try {
                t = std::stoi(s, nullptr, 0);
                if (MinVal <= t && t <= MaxVal) {
                    RefInt = t;
                    return true;
                } else {
                    throw std::invalid_argument("");
                }
            } catch (...) {
                _putts(ErrMsg);
            }
        }
    }
}
