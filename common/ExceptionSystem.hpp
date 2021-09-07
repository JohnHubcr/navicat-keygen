#pragma once
#include <windows.h>
#include "Exception.hpp"

class SystemException : public Exception {
private:
    DWORD _ErrorCode;
    TString _ErrorString;
public:

    template<typename... __Ts>
    SystemException(const TString& FileName, 
                    size_t LineNumber, 
                    DWORD Win32ErrorCode, 
                    const TString& CustomMsg, 
                    __Ts&&... SomeHints) noexcept :
        Exception(FileName, LineNumber, CustomMsg, std::forward<__Ts>(SomeHints)...),
        _ErrorCode(Win32ErrorCode) 
    {
        PTSTR Text = NULL;
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                      NULL,
                      Win32ErrorCode,
                      MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                      reinterpret_cast<PTSTR>(&Text),
                      0,
                      NULL);
        _ErrorString.assign(Text);
        LocalFree(Text);
    }

    virtual bool HasErrorCode() const noexcept override {
        return true;
    }

    virtual uintptr_t ErrorCode() const noexcept override {
        return _ErrorCode;
    }

    virtual const TString& ErrorString() const noexcept override {
        return _ErrorString;
    }
};

