#pragma once
#include <sys/types.h>
#include "NavicatCrypto.hpp"

namespace Helper {

    extern Navicat11Crypto NavicatCipher;

    template<typename _Type, bool _Ascending = true>
    void QuickSort(_Type* pArray, off_t begin, off_t end) {
        if (end - begin <= 1)
            return;

        off_t i = begin;
        off_t j = end - 1;
        _Type seperator = static_cast<_Type&&>(pArray[begin]);

        while (i < j) {
            if (_Ascending) {
                while (i < j && seperator <= pArray[j])
                    --j;

                if (i < j)
                    pArray[i++] = static_cast<_Type&&>(pArray[j]);

                while (i < j && pArray[i] <= seperator)
                    ++i;

                if (i < j)
                    pArray[j--] = static_cast<_Type&&>(pArray[i]);
            } else {
                while (i < j && seperator >= pArray[j])
                    --j;

                if (i < j)
                    pArray[i++] = static_cast<_Type&&>(pArray[j]);

                while (i < j && pArray[i] >= seperator)
                    ++i;

                if (i < j)
                    pArray[j--] = static_cast<_Type&&>(pArray[i]);
            }
        }

        pArray[i] = static_cast<_Type&&>(seperator);
        QuickSort<_Type, _Ascending>(pArray, begin, i);
        QuickSort<_Type, _Ascending>(pArray, i + 1, end);
    }

    //
    //  Print memory data in [from, to) at least
    //  If `base` is not nullptr, print address as offset. Otherwise, as absolute address.
    //  NOTICE:
    //      `base` must <= `from`
    //  
    void PrintMemory(const void* from, const void* to, const void* base = nullptr);
    void PrintBytes(const void* p, size_t s);
    bool IsPrintable(const uint8_t* p, size_t s);

    template<typename __string_t>
    void StringReplace(__string_t& Str, const __string_t& OldSubStr, const __string_t& NewSubStr) {
        typename __string_t::size_type pos = 0;
        auto srclen = OldSubStr.size();
        auto dstlen = NewSubStr.size();

        while ((pos = Str.find(OldSubStr, pos)) != __string_t::npos) {
            Str.replace(pos, srclen, NewSubStr);
            pos += dstlen;
        }
    }

    template<typename __string_t>
    void StringRemove(__string_t& Str, const __string_t& SubStr) {
        typename __string_t::size_type pos = 0;
        while ((pos = Str.find(SubStr, pos)) != __string_t::npos) {
            Str.erase(pos, SubStr.length());
        }
    }
}

