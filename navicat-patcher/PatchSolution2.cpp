#include "PatchSolution.hpp"
#include "Exception.hpp"
#include "Helper.hpp"
#include <tchar.h>

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("PatchSolution2.cpp")

const char PatchSolution2::KeywordsMeta[0x188 + 1] =
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw1dqF3SkCaAAmMzs889I"
    "qdW9M2dIdh3jG9yPcmLnmJiGpBF4E9VHSMGe8oPAy2kJDmdNt4BcEygvssEfginv"
    "a5t5jm352UAoDosUJkTXGQhpAWMF4fBmBpO3EedG62rOsqMBgmSdAyxCSPBRJIOF"
    "R0QgZFbRnU0frj34fiVmgYiLuZSAmIbs8ZxiHPdp1oD4tUpvsFci4QJtYNjNnGU2"
    "WPH6rvChGl1IRKrxMtqLielsvajUjyrgOC6NmymYMvZNER3htFEtL1eQbCyTfDmt"
    "YyQ1Wt4Ot12lxf0wVIR5mcGN7XCXJRHOFHSf1gzXWabRSvmt1nrl7sW6cjxljuuQ"
    "awIDAQAB";

uint8_t PatchSolution2::Keywords[0x188][5];

PatchSolution2::PatchSolution2() {
    for (size_t i = 0; i < 0x188; ++i)
        _PatchOffsets[i] = -1;
}

void PatchSolution2::SetFile(void* pFile) {
    if (!_LibccDllInterpreter.ParseImage(pFile, true)) {
        throw Exception(__BASE_FILE__, __LINE__,
                        TEXT("Invalid PE file."));
    }
}

#if defined(_M_X64)

void PatchSolution2::BuildKeywords() noexcept {
    for (size_t i = 0; i < 0x188; ++i) {
        Keywords[i][0] = 0x83;      // Keywords[i] = asm('xor eax, KeywordsMeta[i]') + 
        Keywords[i][1] = 0xf0;
        Keywords[i][2] = KeywordsMeta[i];
        Keywords[i][3] = 0x88;      //               asm_prefix('mov byte ptr ds:xxxxxxxxxxxxxxxx, al')
        Keywords[i][4] = 0x05;
    }
}

bool PatchSolution2::FindPatchOffset() noexcept {
    PIMAGE_SECTION_HEADER ptextSectionHeader = _LibccDllInterpreter.GetSectionHeader(".text");
    uint8_t* ptextSection = _LibccDllInterpreter.GetSectionView<uint8_t>(".text");
    off_t Offsets[0x188];

    for (size_t i = 0; i < 0x188; ++i)
        Offsets[i] = -1;

    if (ptextSectionHeader == nullptr)
        return false;
    if (ptextSection == nullptr)
        return false;

    BuildKeywords();

    // Find offsets
    {
        size_t FirstKeywordCounter = 0;
        uint32_t Hints[9];
        DWORD PossibleRangeStart = 0xffffffff;
        DWORD PossibleRangeEnd;
        for (DWORD i = 0; i < ptextSectionHeader->SizeOfRawData; ++i) {
            if (memcmp(ptextSection + i, Keywords[0], sizeof(Keywords[0])) == 0) {
                Hints[FirstKeywordCounter++] =
                    *reinterpret_cast<uint32_t*>(ptextSection + i + sizeof(Keywords[0])) +
                    i + sizeof(Keywords[0]) + sizeof(uint32_t);
                if (i < PossibleRangeStart)
                    PossibleRangeStart = i;
            }
        }

        PossibleRangeStart -= 0x1000;
        PossibleRangeEnd = PossibleRangeStart + 0x100000;

        // Keywords[0] should occur 9 times. 
        // Because there's only 9 'M' chars in `KeywordsMeta`.
        if (FirstKeywordCounter != 9)
            return false;

        Helper::QuickSort(Hints, 0, _countof(Hints));

        // assert
        // if not satisfied, refuse to patch
        if (Hints[8] - Hints[0] != 0x18360F8F8 - 0x18360F7D0)
            return false;

        for (size_t i = 0; i < 0x188; ++i) {
            if (Offsets[i] != -1)
                continue;

            for (DWORD j = PossibleRangeStart; j < PossibleRangeEnd; ++j) {
                if (memcmp(ptextSection + j, Keywords[i], sizeof(Keywords[i])) == 0) {
                    off_t index =
                        *reinterpret_cast<uint32_t*>(ptextSection + j + sizeof(Keywords[i])) +
                        j + sizeof(Keywords[i]) + sizeof(uint32_t) - Hints[0];

                    if (0 <= index && index < 0x188 && KeywordsMeta[index] == KeywordsMeta[i]) {
                        Offsets[index] = ptextSectionHeader->PointerToRawData + j;
                    }
                }
            }

            // if not found, refuse to patch
            if (Offsets[i] == -1)
                return false;
        }
    }

    _tprintf_s(TEXT("[*] PatchSolution2 ...... Ready to apply\n"));
    for (size_t i = 0; i < 0x188; ++i) {
        _PatchOffsets[i] = Offsets[i];
        _tprintf_s(TEXT(" |--[*] Keyword[%zu]\n"), i);
        _tprintf_s(TEXT(" |   | - Offset = +0x%.8lx\n"), _PatchOffsets[i]);
        _tprintf_s(TEXT(" |\n"));
    }

    return true;
}

#else

void PatchSolution2::BuildKeywords() noexcept {
    for (size_t i = 0; i < 0x188; ++i) {
        switch (i % 3) {
        case 0:
            Keywords[i][0] = 0x83;      // Keywords[i] = asm('xor edx, KeywordsMeta[i]') + 
            Keywords[i][1] = 0xf2;
            Keywords[i][2] = KeywordsMeta[i];
            Keywords[i][3] = 0x88;      //               asm_prefix('mov byte ptr ds:xxxxxxxx, dl')
            Keywords[i][4] = 0x15;
            break;
        case 1:
            Keywords[i][0] = 0x83;      // Keywords[i] = asm('xor eax, KeywordsMeta[i]') + 
            Keywords[i][1] = 0xf0;
            Keywords[i][2] = KeywordsMeta[i];
            Keywords[i][3] = 0xa2;      //               asm_prefix('mov byte ptr ds:xxxxxxxx, al')
            break;
        default:
            Keywords[i][0] = 0x83;      // Keywords[i] = asm('xor ecx, KeywordsMeta[i]') + 
            Keywords[i][1] = 0xf1;
            Keywords[i][2] = KeywordsMeta[i];
            Keywords[i][3] = 0x88;      //               asm_prefix('mov byte ptr ds:xxxxxxxx, cl')
            Keywords[i][4] = 0x0D;
            break;
        }

    }
}

bool PatchSolution2::FindPatchOffset() noexcept {
    PIMAGE_SECTION_HEADER ptextSectionHeader = _LibccDllInterpreter.GetSectionHeader(".text");
    uint8_t* ptextSection = _LibccDllInterpreter.GetSectionView<uint8_t>(".text");
    off_t Offsets[0x188];

    for (size_t i = 0; i < 0x188; ++i)
        Offsets[i] = -1;

    if (ptextSectionHeader == nullptr)
        return false;
    if (ptextSection == nullptr)
        return false;

    BuildKeywords();

    // Find offsets
    {
        size_t FirstKeywordCounter = 0;
        uint32_t Hints[3];
        DWORD PossibleRangeStart = 0xffffffff;
        DWORD PossibleRangeEnd;
        for (DWORD i = 0; i < ptextSectionHeader->SizeOfRawData; ++i) {
            if (memcmp(ptextSection + i, Keywords[0], sizeof(Keywords[0])) == 0) {
                Hints[FirstKeywordCounter++] =
                    *reinterpret_cast<uint32_t*>(ptextSection + i + sizeof(Keywords[0]));
                if (i < PossibleRangeStart)
                    PossibleRangeStart = i;
            }
        }

        PossibleRangeStart -= 0x1000;
        PossibleRangeEnd = PossibleRangeStart + 0x100000;

        // Keywords[0] should occur 3 times. 
        if (FirstKeywordCounter != 3)
            return false;

        Helper::QuickSort(Hints, 0, _countof(Hints));

        // assert
        // if not satisfied, refuse to patch
        if (Hints[2] - Hints[0] != 0x127382BE - 0x12738210)
            return false;

        for (size_t i = 0; i < 0x188; ++i) {
            uint8_t CurrentKeyword[9];
            size_t CurrentKeywordSize = i % 3 == 1 ? 4 : 5;
            memcpy(CurrentKeyword, Keywords[i], CurrentKeywordSize);
            *reinterpret_cast<uint32_t*>(CurrentKeyword + CurrentKeywordSize) = Hints[0] + i;
            CurrentKeywordSize += sizeof(uint32_t);

            for (DWORD j = PossibleRangeStart; j < PossibleRangeEnd; ++j) {
                if (memcmp(ptextSection + j, CurrentKeyword, CurrentKeywordSize) == 0) {
                    Offsets[i] = ptextSectionHeader->PointerToRawData + j;
                    break;
                }
            }

            // if not found, refuse to patch
            if (Offsets[i] == -1)
                return false;
        }
    }

    _tprintf_s(TEXT("[*] PatchSolution2 ...... Ready to apply\n"));
    for (size_t i = 0; i < 0x188; ++i) {
        _PatchOffsets[i] = Offsets[i];
        _tprintf_s(TEXT(" |--[*] Keyword[%zu]\n"), i);
        _tprintf_s(TEXT(" |   | - Offset = +0x%.8zx\n"), _PatchOffsets[i]);
        _tprintf_s(TEXT(" |"));
    }

    return true;
}

#endif

bool PatchSolution2::CheckKey(RSACipher* pCipher) const {
    std::string PublicKeyString = 
        pCipher->ExportKeyString<RSAKeyType::PublicKey, RSAKeyFormat::PEM>();

    Helper::StringRemove<std::string>(PublicKeyString, "-----BEGIN PUBLIC KEY-----");
    Helper::StringRemove<std::string>(PublicKeyString, "-----END PUBLIC KEY-----");
    Helper::StringRemove<std::string>(PublicKeyString, "\n");

    return PublicKeyString.length() == 0x188;
}

void PatchSolution2::MakePatch(RSACipher* pCipher) const {
    uint8_t* pView = _LibccDllInterpreter.GetImageBase<uint8_t>();

    std::string PublicKeyString = 
        pCipher->ExportKeyString<RSAKeyType::PublicKey, RSAKeyFormat::PEM>();

    Helper::StringRemove<std::string>(PublicKeyString, "-----BEGIN PUBLIC KEY-----");
    Helper::StringRemove<std::string>(PublicKeyString, "-----END PUBLIC KEY-----");
    Helper::StringRemove<std::string>(PublicKeyString, "\n");

    _putts(TEXT("******************************************"));
    _putts(TEXT("*            PatchSulution2              *"));
    _putts(TEXT("******************************************"));

    for (size_t i = 0; i < 0x188; ++i) {
        _tprintf_s(TEXT("@ +0x%.8lx: %.2X %.2X %.2X ---> "),
                   _PatchOffsets[i],
                   pView[_PatchOffsets[i]],
                   pView[_PatchOffsets[i] + 1],
                   pView[_PatchOffsets[i] + 2]);
        pView[_PatchOffsets[i] + 2] = PublicKeyString[i];
        _tprintf_s(TEXT("%.2X %.2X %.2X\n"),
                   pView[_PatchOffsets[i]],
                   pView[_PatchOffsets[i] + 1],
                   pView[_PatchOffsets[i] + 2]);
    }
}

