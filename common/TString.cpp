#include "TString.hpp"
#include "ExceptionSystem.hpp"

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("TString.cpp")

TString TStringBuilder(PCSTR LpMultiByteStr, UINT CodePage) {
#ifdef _UNICODE
    TString TStr;
    int RequiredSize = MultiByteToWideChar(CodePage,
                                           NULL,
                                           LpMultiByteStr,
                                           -1,
                                           NULL,
                                           0);
    if (RequiredSize == 0)
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("MultiByteToWideChar failed."));

    TStr.resize(RequiredSize);

    if (!MultiByteToWideChar(CP_ACP,
                             NULL,
                             LpMultiByteStr,
                             -1,
                             TStr.data(),
                             RequiredSize))
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("MultiByteToWideChar failed."));

    while (TStr.back() == L'\x00')
        TStr.pop_back();

    return TStr;
#else
    if (CodePage == CP_ACP) {
        return TString(LpMultiByteStr);
    } else {
        std::wstring WideCharStr;
        TString TStr;
        int RequiredSize = MultiByteToWideChar(CodePage,
                                               NULL,
                                               LpMultiByteStr,
                                               -1,
                                               NULL,
                                               0);
        if (RequiredSize == 0)
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("MultiByteToWideChar failed."));

        WideCharStr.resize(RequiredSize);

        if (!MultiByteToWideChar(CP_ACP,
                                 NULL,
                                 LpMultiByteStr,
                                 -1,
                                 WideCharStr.data(),
                                 RequiredSize))
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("MultiByteToWideChar failed."));

        while (WideCharStr.back() == L'\x00')
            WideCharStr.pop_back();

        RequiredSize = WideCharToMultiByte(CP_ACP,
                                           NULL,
                                           WideCharStr.c_str(),
                                           -1,
                                           NULL,
                                           0,
                                           NULL,
                                           NULL);
        if (RequiredSize == 0)
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("WideCharToMultiByte failed."));

        TStr.resize(RequiredSize);

        if (!WideCharToMultiByte(CP_ACP,
                                 NULL,
                                 WideCharStr.c_str(),
                                 -1,
                                 TStr.data(),
                                 RequiredSize,
                                 NULL,
                                 NULL))
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("WideCharToMultiByte failed."));

        while (TStr.back() == '\x00')
            TStr.pop_back();

        return TStr;
    }
#endif
}

TString TStringBuilder(PCWSTR LpWideCharStr) {
#ifdef _UNICODE
    return TString(LpWideCharStr);
#else
    TString TStr;
    int RequiredSize = WideCharToMultiByte(CP_ACP,
                                           NULL,
                                           LpWideCharStr,
                                           -1,
                                           NULL,
                                           0,
                                           NULL,
                                           NULL);
    if (RequiredSize == 0)
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    TStr.resize(RequiredSize);

    if (!WideCharToMultiByte(CP_ACP,
                             NULL,
                             LpWideCharStr,
                             -1,
                             TStr.data(),
                             RequiredSize,
                             NULL,
                             NULL))
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    while (TStr.back() == '\x00')
        TStr.pop_back();

    return TStr;
#endif
}

TString TStringBuilder(const std::string& RefMultiByteStr, UINT CodePage) {
    return TStringBuilder(RefMultiByteStr.c_str(), CodePage);
}

TString TStringBuilder(const std::wstring& RefWideCharStr) {
    return TStringBuilder(RefWideCharStr.c_str());
}

std::string TStringEncode(PTSTR LpTString, UINT CodePage) {
    std::string MultiByteStr;
    int RequiredSize;
#ifdef _UNICODE
    RequiredSize = WideCharToMultiByte(CodePage,
                                       NULL,
                                       LpTString,
                                       -1,
                                       NULL,
                                       0,
                                       NULL,
                                       NULL);
    if (RequiredSize == 0)
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    MultiByteStr.resize(RequiredSize);

    if (!WideCharToMultiByte(CodePage,
                             NULL,
                             LpTString,
                             -1,
                             MultiByteStr.data(),
                             RequiredSize,
                             NULL,
                             NULL))
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    while (MultiByteStr.back() == '\x00')
        MultiByteStr.pop_back();
#else
    if (CodePage == CP_ACP)
        return RefTStr;

    std::wstring WideCharStr;
    RequiredSize = MultiByteToWideChar(CP_ACP,
                                       NULL,
                                       LpTString,
                                       -1,
                                       NULL,
                                       0);
    if (RequiredSize == 0)
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("MultiByteToWideChar failed."));

    WideCharStr.resize(RequiredSize);

    if (!MultiByteToWideChar(CP_ACP,
                             NULL,
                             LpTString,
                             -1,
                             WideCharStr.data(),
                             RequiredSize))
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("MultiByteToWideChar failed."));

    while (WideCharStr.back() == L'\x00')
        WideCharStr.pop_back();

    RequiredSize = WideCharToMultiByte(CodePage,
                                       NULL,
                                       WideCharStr.c_str(),
                                       -1,
                                       NULL,
                                       0,
                                       NULL,
                                       NULL);
    if (RequiredSize == 0)
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    MultiByteStr.resize(RequiredSize);

    if (!WideCharToMultiByte(CodePage,
                             NULL,
                             WideCharStr.c_str(),
                             -1,
                             MultiByteStr.data(),
                             RequiredSize,
                             NULL,
                             NULL))
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    while (MultiByteStr.back() == '\x00')
        MultiByteStr.pop_back();
#endif
    return MultiByteStr;
}

std::string TStringEncode(const TString& RefTStr, UINT CodePage) {
    std::string MultiByteStr;
    int RequiredSize;
#ifdef _UNICODE
    RequiredSize = WideCharToMultiByte(CodePage,
                                       NULL,
                                       RefTStr.c_str(),
                                       -1,
                                       NULL,
                                       0,
                                       NULL,
                                       NULL);
    if (RequiredSize == 0)
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    MultiByteStr.resize(RequiredSize);

    if (!WideCharToMultiByte(CodePage,
                             NULL,
                             RefTStr.c_str(),
                             -1,
                             MultiByteStr.data(),
                             RequiredSize,
                             NULL,
                             NULL))
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    while (MultiByteStr.back() == '\x00')
        MultiByteStr.pop_back();
#else
    if (CodePage == CP_ACP)
        return RefTStr;

    std::wstring WideCharStr;
    RequiredSize = MultiByteToWideChar(CP_ACP,
                                       NULL,
                                       RefTStr.c_str(),
                                       -1,
                                       NULL,
                                       0);
    if (RequiredSize == 0)
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("MultiByteToWideChar failed."));

    WideCharStr.resize(RequiredSize);

    if (!MultiByteToWideChar(CP_ACP,
                             NULL,
                             RefTStr.c_str(),
                             -1,
                             WideCharStr.data(),
                             RequiredSize))
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("MultiByteToWideChar failed."));

    while (WideCharStr.back() == L'\x00')
        WideCharStr.pop_back();

    RequiredSize = WideCharToMultiByte(CodePage,
                                       NULL,
                                       WideCharStr.c_str(),
                                       -1,
                                       NULL,
                                       0,
                                       NULL,
                                       NULL);
    if (RequiredSize == 0)
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    MultiByteStr.resize(RequiredSize);

    if (!WideCharToMultiByte(CodePage,
                             NULL,
                             WideCharStr.c_str(),
                             -1,
                             MultiByteStr.data(),
                             RequiredSize,
                             NULL,
                             NULL))
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("WideCharToMultiByte failed."));

    while (MultiByteStr.back() == '\x00')
        MultiByteStr.pop_back();
#endif
    return MultiByteStr;
}

TString TStringDecode(PCSTR LpMultiByteStr, UINT CodePage) {
    return TStringBuilder(LpMultiByteStr, CodePage);
}

TString TStringDecode(const std::string& RefMultiByteStr, UINT CodePage) {
    return TStringBuilder(RefMultiByteStr.c_str(), CodePage);
}

