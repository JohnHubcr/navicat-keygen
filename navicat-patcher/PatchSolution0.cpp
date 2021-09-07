#include "PatchSolution.hpp"
#include "Exception.hpp"
#include "Helper.hpp"
#include <tchar.h>

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("PatchSolution0.cpp")

const char PatchSolution0::Keyword[461] =
    "-----BEGIN PUBLIC KEY-----\r\n"
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw1dqF3SkCaAAmMzs889I\r\n"
    "qdW9M2dIdh3jG9yPcmLnmJiGpBF4E9VHSMGe8oPAy2kJDmdNt4BcEygvssEfginv\r\n"
    "a5t5jm352UAoDosUJkTXGQhpAWMF4fBmBpO3EedG62rOsqMBgmSdAyxCSPBRJIOF\r\n"
    "R0QgZFbRnU0frj34fiVmgYiLuZSAmIbs8ZxiHPdp1oD4tUpvsFci4QJtYNjNnGU2\r\n"
    "WPH6rvChGl1IRKrxMtqLielsvajUjyrgOC6NmymYMvZNER3htFEtL1eQbCyTfDmt\r\n"
    "YyQ1Wt4Ot12lxf0wVIR5mcGN7XCXJRHOFHSf1gzXWabRSvmt1nrl7sW6cjxljuuQ\r\n"
    "awIDAQAB\r\n"
    "-----END PUBLIC KEY-----\r\n";

PatchSolution0::PatchSolution0() :
    _PatchOffset(-1) {}

void PatchSolution0::SetFile(void* pFile) {
    if (!_MainExeInterpreter.ParseImage(pFile, true)) {
        throw Exception(__BASE_FILE__, __LINE__,
                        TEXT("Invalid PE file."));
    }
}

bool PatchSolution0::FindPatchOffset() noexcept {
    _PatchOffset = -1;

    auto pResourceSectionHeader =
        _MainExeInterpreter.GetSectionHeader(".rsrc");
    auto pResourceSection =
        _MainExeInterpreter.GetSectionView<uint8_t>(".rsrc");

    if (pResourceSectionHeader == nullptr || pResourceSection == nullptr)
        return false;

    for (DWORD i = 0; i < pResourceSectionHeader->SizeOfRawData; ++i) {
        if (memcmp(pResourceSection + i, Keyword, 460) == 0) {
            _PatchOffset = pResourceSectionHeader->PointerToRawData + i;
            _tprintf_s(TEXT("[*] PatchSolution0 ...... Ready to apply\n"));
            _tprintf_s(TEXT(" |--[*] Keyword offset = +0x%.8lx\n"), _PatchOffset);
            _tprintf_s(TEXT(" |\n"));
            return true;
        }
    }

    return false;
}

bool PatchSolution0::CheckKey(RSACipher* pCipher) const {
    std::string PublicKeyString = 
        pCipher->ExportKeyString<RSAKeyType::PublicKey, RSAKeyFormat::PEM>();

    Helper::StringReplace<std::string>(PublicKeyString, "\n", "\r\n");

    return PublicKeyString.length() == 460;
}

void PatchSolution0::MakePatch(RSACipher* pCipher) const {
    uint8_t* pView = _MainExeInterpreter.GetImageBase<uint8_t>();

    std::string PublicKeyString =
        pCipher->ExportKeyString<RSAKeyType::PublicKey, RSAKeyFormat::PEM>();
    Helper::StringReplace<std::string>(PublicKeyString, "\n", "\r\n");

    _putts(TEXT("******************************************"));
    _putts(TEXT("*            PatchSulution0              *"));
    _putts(TEXT("******************************************"));
    _tprintf_s(TEXT("@ +0x%.8lx\n"), _PatchOffset);
    _putts(TEXT("Previous:"));
    Helper::PrintMemory(pView + _PatchOffset,
                        pView + _PatchOffset + 460,
                        pView);

    memcpy(pView + _PatchOffset, PublicKeyString.c_str(), 460);

    _putts(TEXT("After:"));
    Helper::PrintMemory(pView + _PatchOffset,
                        pView + _PatchOffset + 460,
                        pView);
}
