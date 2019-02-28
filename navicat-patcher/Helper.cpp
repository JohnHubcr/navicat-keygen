#include "Helper.hpp"
#include <tchar.h>
#include <windows.h>

namespace Helper {

    Navicat11Crypto NavicatCipher("23970790", 8);

    //
    //  read byte(s) at address `p` as _Type to `out`
    //  succeed if return true, otherwise return false
    //
    template<typename _Type>
    static __forceinline bool ProbeForRead(const void* p, void* out) {
        __try {
            *reinterpret_cast<_Type*>(out) = *reinterpret_cast<const _Type*>(p);
            return true;
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }

    void PrintMemory(const void* from, const void* to, const void* base) {
        const uint8_t* start = reinterpret_cast<const uint8_t*>(from);
        const uint8_t* end = reinterpret_cast<const uint8_t*>(to);
        const uint8_t* base_ptr = reinterpret_cast<const uint8_t*>(base);

        if (start >= end)
            return;

        while (reinterpret_cast<uintptr_t>(start) % 16) 
            start--;

        while (reinterpret_cast<uintptr_t>(start) % 16) 
            end++;

        while (start < end) {
            uint16_t value[16] = {};

            if (base_ptr) {
                uintptr_t d = start >= base ? start - base_ptr : base_ptr - start;
                if (start >= base) {
                    if constexpr (sizeof(void*) == 4) {
                        _tprintf(TEXT("+0x%.8zx  "), d);
                    }
                    if constexpr (sizeof(void*) == 8) {
                        _tprintf(TEXT("+0x%.16zx  "), d);
                    }
                } else {
                    if constexpr (sizeof(void*) == 4) {
                        _tprintf(TEXT("-0x%.8zx  "), d);
                    }
                    if constexpr (sizeof(void*) == 8) {
                        _tprintf(TEXT("-0x%.16zx  "), d);
                    }
                }
            } else {
                _tprintf(TEXT("0x%p  "), start);
            }

            for (int i = 0; i < 16; ++i) {
                if (ProbeForRead<uint8_t>(start + i, value + i)) {
                    _tprintf(TEXT("%.2x "), value[i]);
                } else {
                    value[i] = -1;
                    _tprintf(TEXT("?? "));
                }
            }

            _tprintf(TEXT(" "));

            for (int i = 0; i < 16; ++i) {
                if (value[i] < 0x20) {
                    _tprintf(TEXT("."));
                } else if (value[i] > 0x7e) {
                    _tprintf(TEXT("."));
                } else {
                    _tprintf(TEXT("%c"), value[i]);
                }
            }

            _tprintf(TEXT("\n"));

            start += 0x10;
        }
    }

    void PrintBytes(const void* p, size_t s) {
        const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(p);

        if (s == 0)
            return;

        if (s == 1) {
            _tprintf_s(TEXT("%.2X"), byte_ptr[0]);
            return;
        }

        s -= 1;
        for (size_t i = 0; i < s; ++i)
            _tprintf_s(TEXT("%.2X "), byte_ptr[i]);

        _tprintf_s(TEXT("%.2X"), byte_ptr[s]);
    }

    bool IsPrintable(const uint8_t* p, size_t s) {
        for (size_t i = 0; i < s; ++i)
            if (isprint(p[i]) == false)
                return false;
        return true;
    }
}
