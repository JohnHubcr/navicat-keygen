#include "PatchSolution.hpp"
#include "Exception.hpp"
#include "Helper.hpp"
#include <tchar.h>

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("PatchSolution1.cpp")

const PatchSolution1::KeywordInfo PatchSolution1::Keywords[5] = {
    {
        "D75125B70767B94145B47C1CB3C0755E"
        "7CCB8825C5DCE0C58ACF944E08280140"
        "9A02472FAFFD1CD77864BB821AE36766"
        "FEEDE6A24F12662954168BFA314BD950"
        "32B9D82445355ED7BC0B880887D650F5",
        160,
        STRING_DATA
    },

    {
        "\xfe\xea\xbc\x01",
        4,
        IMM_DATA
    },

    {
        "E1CED09B9C2186BF71A70C0FE2F1E0AE"
        "F3BD6B75277AAB20DFAF3D110F75912B"
        "FB63AC50EC4C48689D1502715243A79F"
        "39FF2DE2BF15CE438FF885745ED54573"
        "850E8A9F40EE2FF505EB7476F95ADB78"
        "3B28CA374FAC4632892AB82FB3BF4715"
        "FCFE6E82D03731FC3762B6AAC3DF1C3B"
        "C646FE9CD3C62663A97EE72DB932A301"
        "312B4A7633100C8CC357262C39A2B3A6"
        "4B224F5276D5EDBDF0804DC3AC4B8351"
        "62BB1969EAEBADC43D2511D6E0239287"
        "81B167A48273B953378D3D2080CC0677"
        "7E8A2364F0234B81064C5C739A8DA28D"
        "C5889072BF37685CBC94C2D31D0179AD"
        "86D8E3AA8090D4F0B281BE37E0143746"
        "E6049CCC06899401264FA471C016A96C"
        "79815B55BBC26B43052609D9D175FBCD"
        "E455392F10E51EC162F51CF732E6BB39"
        "1F56BBFD8D957DF3D4C55B71CEFD54B1"
        "9C16D458757373E698D7E693A8FC3981"
        "5A8BF03BA05EA8C8778D38F9873D62B4"
        "460F41ACF997C30E7C3AF025FA171B5F"
        "5AD4D6B15E95C27F6B35AD61875E5505"
        "449B4E",
        742,
        STRING_DATA
    },
        
    {
        "\x59\x08\x01\x00",
        4,
        IMM_DATA
    },
        
    {
        "92933",
        5,
        STRING_DATA
    }
};

PatchSolution1::PatchSolution1() :
    _Patches{} {}

void PatchSolution1::SetFile(void* pFile) {
    if (!_LibccDllInterpreter.ParseImage(pFile, true)) {
        throw Exception(__BASE_FILE__, __LINE__,
                        TEXT("Invalid PE file."));
    }
}

bool PatchSolution1::FindPatchOffset() noexcept {
    PIMAGE_SECTION_HEADER ptextSectionHeader = _LibccDllInterpreter.GetSectionHeader(".text");
    PIMAGE_SECTION_HEADER prdataSectionHeader = _LibccDllInterpreter.GetSectionHeader(".rdata");
    uint8_t* ptextSection = _LibccDllInterpreter.GetSectionView<uint8_t>(".text");
    uint8_t* prdataSection = _LibccDllInterpreter.GetSectionView<uint8_t>(".rdata");
    PatchPointInfo TempPatches[5] = {};

    if (ptextSectionHeader == nullptr)
        return false;
    if (prdataSectionHeader == nullptr)
        return false;

    // -------------------------
    // try to search Keywords[0]
    // -------------------------
    for (DWORD i = 0; i < prdataSectionHeader->SizeOfRawData; ++i) {
        if (memcmp(prdataSection + i, Keywords[0].Data, Keywords[0].Length) == 0) {
            TempPatches[0].Ptr = prdataSection + i;

            size_t j = Keywords[0].Length;
            while (TempPatches[0].Ptr[j] == 0)
                ++j;

            TempPatches[0].MaxPatchSize = j - 1;
            break;
        }
    }

    if (TempPatches[0].Ptr == nullptr)
        return false;

    // -------------------------
    // try to search Keywords[2]
    // -------------------------
    for (DWORD i = 0; i < prdataSectionHeader->SizeOfRawData; ++i) {
        if (memcmp(prdataSection + i, Keywords[2].Data, Keywords[2].Length) == 0) {
            TempPatches[2].Ptr = prdataSection + i;

            size_t j = Keywords[2].Length;
            while (TempPatches[2].Ptr[j] == 0)
                ++j;

            TempPatches[2].MaxPatchSize = j - 1;
            break;
        }
    }

    if (TempPatches[2].Ptr == nullptr)
        return false;

    // -------------------------
    // try to search Keywords[4]
    // -------------------------
    for (DWORD i = 0; i < prdataSectionHeader->SizeOfRawData; ++i) {
        if (memcmp(prdataSection + i, Keywords[4].Data, Keywords[4].Length) == 0) {
            TempPatches[4].Ptr = prdataSection + i;

            size_t j = Keywords[4].Length;
            while (TempPatches[4].Ptr[j] == 0)
                ++j;

            TempPatches[4].MaxPatchSize = j - 1;
            break;
        }
    }

    if (TempPatches[4].Ptr == nullptr)
        return false;

    // -------------------------
    // try to search Keywords[1] and Keywords[3]
    // -------------------------
    for (DWORD i = 0; i < ptextSectionHeader->SizeOfRawData; ++i) {
        if (memcmp(ptextSection + i, Keywords[1].Data, Keywords[1].Length) == 0) {

            // Keywords[3] must be close to Keywords[1]
            for (DWORD j = i - 64; j < i + 64; ++j) {
                if (memcmp(ptextSection + j, Keywords[3].Data, Keywords[3].Length) == 0) {
                    TempPatches[1].Ptr = ptextSection + i;
                    TempPatches[1].PatchSize = Keywords[1].Length;
                    TempPatches[1].MaxPatchSize = Keywords[1].Length;
                    
                    TempPatches[3].Ptr = ptextSection + j;
                    TempPatches[3].PatchSize = Keywords[3].Length;
                    TempPatches[3].MaxPatchSize = Keywords[3].Length;
                    break;
                }
            }
            
            if (TempPatches[3].Ptr)
                break;
        }
    }

    if (TempPatches[1].Ptr == nullptr || TempPatches[3].Ptr == nullptr)
        return false;

    _tprintf_s(TEXT("[*] PatchSolution1 ...... Ready to apply\n"));
    for (size_t i = 0; i < 5; ++i) {
        _Patches[i] = TempPatches[i];
        _tprintf_s(TEXT(" |--[*] Keyword[%zu]\n"), i);
        _tprintf_s(TEXT(" |   | - Offset = +0x%.8zx\n"), _Patches[i].Ptr - _LibccDllInterpreter.GetImageBase<uint8_t>());
        _tprintf_s(TEXT(" |\n"));
    }
    return true;
}

bool PatchSolution1::CheckKey(RSACipher* pCipher) const {
    std::string PublicKeyString = 
        pCipher->ExportKeyString<RSAKeyType::PublicKey, RSAKeyFormat::PEM>();
    Helper::StringReplace<std::string>(PublicKeyString, "\n", "\r\n");

    std::string EncryptedKeyString = Helper::NavicatCipher.EncryptString(PublicKeyString);

    if (EncryptedKeyString.length() != 920)
        return false;

    // we require the chars in [p1, p2) of EncryptedKeyString must be number chars
    size_t p1, p2;

    p1 = _Patches[0].MaxPatchSize;
    p2 = Keywords[0].Length + 8;        // 8 = strlen("29158142")
    if (p1 >= p2)
        p1 = p2 - 1;

    if (('1' <= EncryptedKeyString[p1] && EncryptedKeyString[p1] <= '9') == false)
        return false;

    for (size_t i = p1 + 1; i < p2; ++i)
        if (('0' <= EncryptedKeyString[i] && EncryptedKeyString[i] <= '9') == false)
            return false;

    _Patches[0].PatchSize = p1;

    p1 = Keywords[0].Length + 8 + _Patches[2].MaxPatchSize;
    p2 = Keywords[0].Length + 8 + Keywords[2].Length + 5;
    if (p1 >= p2)
        p1 = p2 - 1;

    if (('1' <= EncryptedKeyString[p1] && EncryptedKeyString[p1] <= '9') == false)
        return false;

    for (size_t i = p1 + 1; i < p2; ++i)
        if (('0' <= EncryptedKeyString[i] && EncryptedKeyString[i] <= '9') == false)
            return false;

    _Patches[2].PatchSize = p1 - Keywords[0].Length - 8;

    _Patches[4].PatchSize = Keywords[4].Length;
    return true;
}

void PatchSolution1::MakePatch(RSACipher* pCipher) const {
    uint8_t* pView = _LibccDllInterpreter.GetImageBase<uint8_t>();

    std::string PublicKeyString = 
        pCipher->ExportKeyString<RSAKeyType::PublicKey, RSAKeyFormat::PEM>();
    Helper::StringReplace<std::string>(PublicKeyString, "\n", "\r\n");

    std::string EncryptedKeyString = Helper::NavicatCipher.EncryptString(PublicKeyString);

    // split EncryptedKeyString to 5 part:    |160 chars|8 chars|742 chars|5 chars|5 chars|
    //                                                       |                |
    //                                                      \ /              \ /
    //                                                   ImmValue1        ImmValue3
    size_t p0, p1, p2, p3, p4, p5;
    p0 = 0;
    p1 = p0 + _Patches[0].PatchSize;
    p2 = Keywords[0].Length + 8;
    p3 = p2 + _Patches[2].PatchSize;
    p4 = Keywords[0].Length + 8 + Keywords[2].Length + 5;
    p5 = 920;

    std::string EncryptedPEM0(EncryptedKeyString.begin() + p0, EncryptedKeyString.begin() + p1);
    std::string EncryptedPEM1(EncryptedKeyString.begin() + p1, EncryptedKeyString.begin() + p2);
    std::string EncryptedPEM2(EncryptedKeyString.begin() + p2, EncryptedKeyString.begin() + p3);
    std::string EncryptedPEM3(EncryptedKeyString.begin() + p3, EncryptedKeyString.begin() + p4);
    std::string EncryptedPEM4(EncryptedKeyString.begin() + p4, EncryptedKeyString.begin() + p5);
    uint32_t ImmValue1 = std::stoul(EncryptedPEM1.c_str());
    uint32_t ImmValue3 = std::stoul(EncryptedPEM3.c_str());

    _putts(TEXT("******************************************"));
    _putts(TEXT("*            PatchSulution1              *"));
    _putts(TEXT("******************************************"));

    // ----------------------------------
    //     process PatchOffsets[0]
    // ----------------------------------
    _tprintf_s(TEXT("@ +0x%.8zx\n"), _Patches[0].Ptr - pView);
    _putts(TEXT("Previous:"));
    Helper::PrintMemory(_Patches[0].Ptr,
                        _Patches[0].Ptr + _Patches[0].PatchSize,
                        pView);
    memcpy(_Patches[0].Ptr, EncryptedPEM0.c_str(), _Patches[0].PatchSize);
    _putts(TEXT("After:"));
    Helper::PrintMemory(_Patches[0].Ptr,
                        _Patches[0].Ptr + _Patches[0].PatchSize,
                        pView);
    _putts(TEXT(""));

    // ----------------------------------
    //     process PatchOffsets[1]
    // ----------------------------------
    _tprintf_s(TEXT("@ +0x%.8zx\n"), _Patches[1].Ptr - pView);
    _putts(TEXT("Previous:"));
    Helper::PrintMemory(_Patches[1].Ptr,
                        _Patches[1].Ptr + _Patches[1].PatchSize,
                        pView);
    memcpy(_Patches[1].Ptr, &ImmValue1, sizeof(uint32_t));
    _putts(TEXT("After:"));
    Helper::PrintMemory(_Patches[1].Ptr,
                        _Patches[1].Ptr + _Patches[1].PatchSize,
                        pView);
    _putts(TEXT(""));

    // ----------------------------------
    //     process PatchOffsets[2]
    // ----------------------------------
    _tprintf_s(TEXT("@ +0x%.8zx\n"), _Patches[2].Ptr - pView);
    _putts(TEXT("Previous:"));
    Helper::PrintMemory(_Patches[2].Ptr,
                        _Patches[2].Ptr + _Patches[2].PatchSize,
                        pView);
    memcpy(_Patches[2].Ptr, EncryptedPEM2.c_str(), _Patches[2].PatchSize);
    _putts(TEXT("After:"));
    Helper::PrintMemory(_Patches[2].Ptr,
                        _Patches[2].Ptr + _Patches[2].PatchSize,
                        pView);
    _putts(TEXT(""));

    // ----------------------------------
    //     process PatchOffsets[3]
    // ----------------------------------
    _tprintf_s(TEXT("@ +0x%.8zx\n"), _Patches[3].Ptr - pView);
    _putts(TEXT("Previous:"));
    Helper::PrintMemory(_Patches[3].Ptr,
                        _Patches[3].Ptr + _Patches[3].PatchSize,
                        pView);
    memcpy(_Patches[3].Ptr, &ImmValue3, sizeof(uint32_t));
    _putts(TEXT("After:"));
    Helper::PrintMemory(_Patches[3].Ptr,
                        _Patches[3].Ptr + _Patches[3].PatchSize,
                        pView);
    _putts(TEXT(""));

    // ----------------------------------
    //     process PatchOffsets[4]
    // ----------------------------------
    _tprintf_s(TEXT("@ +0x%.8zx\n"), _Patches[4].Ptr - pView);
    _putts(TEXT("Previous:"));
    Helper::PrintMemory(_Patches[4].Ptr,
                        _Patches[4].Ptr + _Patches[4].PatchSize,
                        pView);
    memcpy(_Patches[4].Ptr, EncryptedPEM4.c_str(), _Patches[4].PatchSize);
    _putts(TEXT("After:"));
    Helper::PrintMemory(_Patches[4].Ptr,
                        _Patches[4].Ptr + _Patches[4].PatchSize,
                        pView);
}

