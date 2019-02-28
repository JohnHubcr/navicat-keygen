#pragma once
#include <stddef.h>
#include <stdint.h>
#include <windows.h>
#include <map>

class ImageInterpreter {
private:
    PVOID _ImageBasePtr;
    PIMAGE_NT_HEADERS _NTHeadersPtr;
    PIMAGE_SECTION_HEADER _SectionHeaderTablePtr;
    std::map<uint64_t, size_t> _SectionNameTable;
    std::map<uintptr_t, size_t> _SectionMapAddressTable;
    std::map<uintptr_t, size_t> _RelocationAddressTable;
public:
    ImageInterpreter();

    bool ParseImage(const PVOID PtrToImageBase, bool DisableRelocParsing);

    template<typename __Type>
    __Type* GetImageBase() const {
        return reinterpret_cast<__Type*>(_ImageBasePtr);
    }

    PIMAGE_DOS_HEADER GetImageDosHeader() const;
    PIMAGE_NT_HEADERS GetImageNTHeaders() const;
    PIMAGE_SECTION_HEADER GetSectionHeaderTable() const;
    PIMAGE_SECTION_HEADER GetSectionHeader(const char* SectionName) const;
    PIMAGE_SECTION_HEADER GetSectionHeader(uintptr_t Rva) const;

    template<typename __Type>
    __Type* GetSectionView(const char* SectionName) const {
        auto PtrToSectionHeader = GetSectionHeader(SectionName);

        if (PtrToSectionHeader == nullptr)
            return nullptr;

        return reinterpret_cast<__Type*>(
            reinterpret_cast<uint8_t*>(_ImageBasePtr) +
            PtrToSectionHeader->PointerToRawData
        );
    }

    template<typename __Type>
    __Type* GetSectionView(uintptr_t Rva) const {
        auto PtrToSectionHeader = GetSectionHeader(Rva);

        if (PtrToSectionHeader == nullptr)
            return nullptr;

        return reinterpret_cast<__Type*>(
            reinterpret_cast<uint8_t*>(_ImageBasePtr) +
            PtrToSectionHeader->PointerToRawData
        );
    }

    template<typename __Type>
    __Type* RvaToPointer(uintptr_t Rva) const {
        auto PtrToSectionHeader = GetSectionHeader(Rva);

        if (PtrToSectionHeader == nullptr)
            return nullptr;

        uint8_t* SectionViewPtr =
            reinterpret_cast<uint8_t*>(_ImageBasePtr) +
            PtrToSectionHeader->PointerToRawData;

        return reinterpret_cast<__Type*>(
            SectionViewPtr + (Rva - PtrToSectionHeader->VirtualAddress)
        );
    }

    bool IsRvaRangeInRelocTable(uintptr_t Rva, size_t Size) const;
};

