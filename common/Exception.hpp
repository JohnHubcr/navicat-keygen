#pragma once
#include "TString.hpp"
#include <vector>

class Exception {
private:
    static inline TString EmptyErrorStr;
    TString _File;
    size_t _Line;
    TString _Message;
    std::vector<TString> _Hints;
public:

    template<typename... __Ts>
    Exception(const TString& FileName, 
              size_t LineNumber, 
              const TString& CustomMsg, 
              __Ts&&... SomeHints) noexcept :
        _File(FileName),
        _Line(LineNumber),
        _Message(CustomMsg),
        _Hints{ std::forward<__Ts>(SomeHints)... } {}

    const TString& File() const noexcept {
        return _File;
    }

    size_t Line() const noexcept {
        return _Line;
    }

    const TString& Message() const noexcept {
        return _Message;
    }

    std::vector<TString>& Hints() noexcept {
        return _Hints;
    }

    const std::vector<TString>& Hints() const noexcept {
        return _Hints;
    }

    virtual bool HasErrorCode() const noexcept {
        return false;
    }

    virtual uintptr_t ErrorCode() const noexcept {
        return 0;
    }

    virtual const TString& ErrorString() const noexcept {
        return EmptyErrorStr;
    }

    virtual ~Exception() = default;
};

