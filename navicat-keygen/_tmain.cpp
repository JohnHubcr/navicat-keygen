#include <tchar.h>
#include <windows.h>
#include "ResourceObject.hpp"
#include "RSACipher.hpp"

void NormalMode(RSACipher* pCipher, bool bGenerateText);
void AdvanceMode(RSACipher* pCipher, bool bGenerateText);

static void Welcome() {
    _putts(TEXT("***************************************************"));
    _putts(TEXT("*       Navicat Keygen by @DoubleLabyrinth        *"));
    _putts(TEXT("*                   Version: 3.2                  *"));
    _putts(TEXT("***************************************************"));
    _putts(TEXT(""));
}

static void Help() {
    _putts(TEXT("Usage:"));
    _putts(TEXT("    navicat-keygen.exe <-bin|-text> [-adv] <RSA-2048 Private Key File>"));
    _putts(TEXT(""));
    _putts(TEXT("    <-bin|-text>       Specify \"-bin\" to generate \"license_file\" used by Navicat 11."));
    _putts(TEXT("                       Specify \"-text\" to generate base64-encoded activation code."));
    _putts(TEXT("                       This parameter must be specified."));
    _putts(TEXT(""));
    _putts(TEXT("    [-adv]             Enable advance mode."));
    _putts(TEXT("                       This parameter is optional."));
    _putts(TEXT(""));
    _putts(TEXT("    <RSA-2048 Private Key File>    A path to an RSA-2048 private key file."));
    _putts(TEXT("                                   This parameter must be specified."));
    _putts(TEXT(""));
    _putts(TEXT("Example:"));
    _putts(TEXT("    navicat-keygen.exe -text .\\RegPrivateKey.pem"));
}

int _tmain(int argc, PTSTR argv[]) {
    if (argc != 3 && argc != 4) {
        Help();
        return 0;
    }
    
    Welcome();

    bool bGenerateText;
    bool bAdvanceMode;
    ResourceObject<CppObjectTraits<RSACipher>> pCipher;

    if (_tcsicmp(argv[1], TEXT("-bin")) == 0) {
        bGenerateText = false;
    } else if (_tcsicmp(argv[1], TEXT("-text")) == 0) {
        bGenerateText = true;
    } else {
        Help();
        return 0;
    }

    if (argc == 4) {
        if (_tcsicmp(argv[2], TEXT("-adv")) == 0) {
            bAdvanceMode = true;
        } else {
            Help();
            return 0;
        }
    } else {
        bAdvanceMode = false;
    }

    pCipher.TakeOver(new RSACipher());

    try {
        pCipher->ImportKeyFromFile<RSAKeyType::PrivateKey, RSAKeyFormat::PEM>(TStringEncode(argv[argc - 1], CP_ACP));
        if (bAdvanceMode) {
            AdvanceMode(pCipher, bGenerateText);
        } else {
            NormalMode(pCipher, bGenerateText);
        }
    } catch (Exception& e) {
        _tprintf_s(TEXT("[*] ERROR: %s\n"), e.Message().c_str());
        _tprintf_s(TEXT(" | - File: %s\n"), e.File().c_str());
        _tprintf_s(TEXT(" | - Line: %zu\n"), e.Line());
        if (e.HasErrorCode()) {
            _tprintf_s(TEXT(" | - ErrorCode: 0x%.8zx\n"), e.ErrorCode());
            _tprintf_s(TEXT(" | - ErrorString: %s\n"), e.ErrorString().c_str());
        }

        if (e.Hints().size() != 0) {
            _putts(TEXT(" | - Hints:"));
            for (auto& Hint : e.Hints())
                _tprintf_s(TEXT(" |   %s\n"), Hint.c_str());
        }

        _putts(TEXT(""));
    }
    return 0;
}
