#pragma once
#include "Exception.hpp"
#include <capstone/capstone.h>

class CapstoneException : public Exception {
private:
    cs_err _ErrorCode;
    TString _ErrorString;
public:

    template<typename... __Ts>
    CapstoneException(const TString& FileName,
                      size_t LineNumber,
                      cs_err ErrCode,
                      const TString& CustomMsg, __Ts&&... SomeHints) noexcept :
        Exception(FileName, LineNumber, CustomMsg, std::forward<__Ts>(SomeHints)...),
        _ErrorCode(ErrCode),
        _ErrorString(TStringBuilder(cs_strerror(ErrCode))) {}

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

