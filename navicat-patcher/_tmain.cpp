#include <tchar.h>
#include <windows.h>
#include "Exception.hpp"
#include "ExceptionSystem.hpp"
#include "FileManipulation.hpp"
#include "PatchSolution.hpp"

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("_tmain.cpp")

#define PRINT_MESSAGE_LITERAL(m) _putts(TEXT(m))
#define PRINT_PTSTR(m) _putts(m)
#define PRINT_PCSTR(m)  puts(m)
#define PRINT_PCWSTR(m) _putws(m)

static void Welcome() {
    _putts(TEXT("***************************************************"));
    _putts(TEXT("*       Navicat Patcher by @DoubleLabyrinth       *"));
    _putts(TEXT("*                   Version: 3.2                  *"));
    _putts(TEXT("***************************************************"));
    _putts(TEXT(""));
    _putts(TEXT("Press Enter to continue or Ctrl + C to abort."));
    _gettchar();
}

static void Help() {
    _putts(TEXT("Usage:"));
    _putts(TEXT("    navicat-patcher.exe <Navicat Installation Path> [RSA-2048 PEM File Path]"));
    _putts(TEXT(""));
    _putts(TEXT("    <Navicat Installation Path>  The folder path where Navicat is installed."));
    _putts(TEXT("                                 This parameter must be specified."));
    _putts(TEXT(""));
    _putts(TEXT("    [RSA-2048 PEM File Path]     The path to an RSA-2048 private key file."));
    _putts(TEXT("                                 This parameter is optional."));
    _putts(TEXT("                                 If not specified, an RSA-2048 private key file"));
    _putts(TEXT("                                 named \"RegPrivateKey.pem\" will be generated."));
    _putts(TEXT(""));
    _putts(TEXT("Example:"));
    _putts(TEXT("    navicat-patcher.exe \"C:\\Program Files\\PremiumSoft\\Navicat Premium 12\""));
}

static void BackupFile(const TString& From, const TString& To) {
    if (::CopyFile(From.c_str(), To.c_str(), TRUE) == FALSE)
        throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                              TEXT("CopyFile failed."));
}

static void LoadKey(RSACipher* pCipher, PTSTR pKeyFileName, 
                    PatchSolution* pSolution0,
                    PatchSolution* pSolution1, 
                    PatchSolution* pSolution2, 
                    PatchSolution* pSolution3) {
    if (pKeyFileName) {
        _tprintf_s(TEXT("[*] Import RSA-2048 key from file %s\n"), pKeyFileName);

        pCipher->ImportKeyFromFile<RSAKeyType::PrivateKey, RSAKeyFormat::PEM>(TStringEncode(pKeyFileName, CP_UTF8));

        if (pSolution0 && !pSolution0->CheckKey(pCipher) ||
            pSolution1 && !pSolution1->CheckKey(pCipher) ||
            pSolution2 && !pSolution2->CheckKey(pCipher) ||
            pSolution3 && !pSolution3->CheckKey(pCipher))
            throw Exception(__BASE_FILE__, __LINE__, 
                            TEXT("The RSA private key you provide cannot be used."));
    } else {
        _putts(TEXT("[*] Generating new RSA private key, it may take a long time."));

        do {
            pCipher->GenerateKey(2048);
        } while (pSolution0 && !pSolution0->CheckKey(pCipher) ||
                 pSolution1 && !pSolution1->CheckKey(pCipher) ||
                 pSolution2 && !pSolution2->CheckKey(pCipher) ||
                 pSolution3 && !pSolution3->CheckKey(pCipher));   // re-generate RSA key if one of 'CheckKey's return false

        pCipher->ExportKeyToFile<RSAKeyType::PrivateKey, RSAKeyFormat::NotSpecified>("RegPrivateKey.pem");
        _putts(TEXT("[*] New RSA-2048 private key has been saved to RegPrivateKey.pem."));
    }

    std::string PublicKeyString = pCipher->ExportKeyString<RSAKeyType::PublicKey, RSAKeyFormat::PEM>();

    _putts(TEXT("[*] Your RSA public key:"));
    printf_s("\n%s\n", PublicKeyString.c_str());
}

static void ExceptionReport(const Exception& e) noexcept {
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

int _tmain(int argc, PTSTR argv[]) {
    if (argc != 2 && argc != 3) {
        Help();
        return 0;
    }

    Welcome();

    TString InstallationPath;
    TString MainExeName;
    TString LibccDllName;
    ResourceObject<CppObjectTraits<RSACipher>> pCipher;
    ResourceObject<CppObjectTraits<FileObject>> MainExe; 
    ResourceObject<CppObjectTraits<FileMapViewObject>> MainExeMapper;
    ResourceObject<CppObjectTraits<FileObject>> LibccDll;
    ResourceObject<CppObjectTraits<FileMapViewObject>> LibccDllMapper;
    ResourceObject<CppObjectTraits<PatchSolution>> pSolution0;
    ResourceObject<CppObjectTraits<PatchSolution>> pSolution1;
    ResourceObject<CppObjectTraits<PatchSolution>> pSolution2;
    ResourceObject<CppObjectTraits<PatchSolution>> pSolution3;

    try {
        DWORD Attribute = GetFileAttributes(argv[1]);
        if (Attribute == INVALID_FILE_ATTRIBUTES)
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("GetFileAttributes failed."));

        if ((Attribute & FILE_ATTRIBUTE_DIRECTORY) == 0)
            throw Exception(__BASE_FILE__, __LINE__,
                            TEXT("Path does not point to a directory."));

        InstallationPath = argv[1];
        if (InstallationPath.back() != TEXT('\\') && InstallationPath.back() != TEXT('/'))
            InstallationPath.push_back(TEXT('/'));  // for Linux compatible
    } catch (Exception& e) {
        e.Hints().emplace_back(TEXT("Are you sure the path you specified is correct?"));
        e.Hints().emplace_back(TEXT("The path you specified:"));
        e.Hints().emplace_back(argv[1]);
        ExceptionReport(e);
        return -1;
    }

    pCipher.TakeOver(new RSACipher());
    
    // -----------------
    //  Map files
    // -----------------
    do {
        try {
            _tprintf_s(TEXT("[*] Try to open libcc.dll   ...... "));
            LibccDll.TakeOver(FileObject::Open<FileObject*>(InstallationPath + TEXT("libcc.dll"), 
                                                            GENERIC_READ | GENERIC_WRITE, 
                                                            FILE_SHARE_READ));
            LibccDllMapper.TakeOver(FileMapViewObject::Create<FileMapViewObject*>(LibccDll->GetHandle(), 
                                                                                  PAGE_READWRITE));
            LibccDllMapper->Map(FILE_MAP_READ | FILE_MAP_WRITE);
            LibccDllName = TEXT("libcc.dll");
            _tprintf_s(TEXT("Succeeded\n"));
            break;
        } catch (Exception& e) {
            if (e.HasErrorCode() && e.ErrorCode() == ERROR_FILE_NOT_FOUND) {
                _tprintf_s(TEXT("Not found\n"));
                LibccDllMapper.Release();
                LibccDll.Release();
            } else {
                _tprintf_s(TEXT("Failed\n"));
                if (e.HasErrorCode() && e.ErrorCode() == ERROR_ACCESS_DENIED)
                    e.Hints().emplace_back(TEXT("Please re-run with Administrator privilege."));
                ExceptionReport(e);
                return -1;
            }
        }

        try {
            _tprintf_s(TEXT("[*] Try to open Navicat.exe ...... "));
            MainExe.TakeOver(FileObject::Open<FileObject*>(InstallationPath + TEXT("Navicat.exe"),
                                                           GENERIC_READ | GENERIC_WRITE,
                                                           FILE_SHARE_READ));
            MainExeMapper.TakeOver(FileMapViewObject::Create<FileMapViewObject*>(MainExe->GetHandle(),
                                                                                 PAGE_READWRITE));
            MainExeMapper->Map(FILE_MAP_READ | FILE_MAP_WRITE);
            MainExeName = TEXT("Navicat.exe");
            _tprintf_s(TEXT("Succeeded\n"));
            break;
        } catch (Exception& e) {
            if (e.HasErrorCode() && e.ErrorCode() == ERROR_FILE_NOT_FOUND) {
                _tprintf_s(TEXT("Not found\n"));
                MainExeMapper.Release();
                MainExe.Release();
            } else {
                _tprintf_s(TEXT("Failed\n"));
                if (e.HasErrorCode() && e.ErrorCode() == ERROR_ACCESS_DENIED)
                    e.Hints().emplace_back(TEXT("Please re-run with Administrator privilege."));
                ExceptionReport(e);
                return -1;
            }
        }

        try {
            _tprintf_s(TEXT("[*] Try to open Modeler.exe ...... "));
            MainExe.TakeOver(FileObject::Open<FileObject*>(InstallationPath + TEXT("Modeler.exe"),
                                                           GENERIC_READ | GENERIC_WRITE,
                                                           FILE_SHARE_READ));
            MainExeMapper.TakeOver(FileMapViewObject::Create<FileMapViewObject*>(MainExe->GetHandle(),
                                                                                 PAGE_READWRITE));
            MainExeMapper->Map(FILE_MAP_READ | FILE_MAP_WRITE);
            MainExeName = TEXT("Modeler.exe");
            _tprintf_s(TEXT("Succeeded\n"));
            break;
        } catch (Exception& e) {
            if (e.HasErrorCode() && e.ErrorCode() == ERROR_FILE_NOT_FOUND) {
                _tprintf_s(TEXT("Not found\n"));
                MainExeMapper.Release();
                MainExe.Release();
            } else {
                _tprintf_s(TEXT("Failed\n"));
                if (e.HasErrorCode() && e.ErrorCode() == ERROR_ACCESS_DENIED)
                    e.Hints().emplace_back(TEXT("Please re-run with Administrator privilege."));
                ExceptionReport(e);
                return -1;
            }
        }

        try {
            _tprintf_s(TEXT("[*] Try to open Rviewer.exe ...... "));
            MainExe.TakeOver(FileObject::Open<FileObject*>(InstallationPath + TEXT("Rviewer.exe"),
                                                           GENERIC_READ | GENERIC_WRITE,
                                                           FILE_SHARE_READ));
            MainExeMapper.TakeOver(FileMapViewObject::Create<FileMapViewObject*>(MainExe->GetHandle(),
                                                                                 PAGE_READWRITE));
            MainExeMapper->Map(FILE_MAP_READ | FILE_MAP_WRITE);
            MainExeName = TEXT("Rviewer.exe");
            _tprintf_s(TEXT("Succeeded\n"));
            break;
        } catch (Exception& e) {
            if (e.HasErrorCode() && e.ErrorCode() == ERROR_FILE_NOT_FOUND) {
                _tprintf_s(TEXT("Not found\n"));
                MainExeMapper.Release();
                MainExe.Release();
            } else {
                _tprintf_s(TEXT("Failed\n"));
                if (e.HasErrorCode() && e.ErrorCode() == ERROR_ACCESS_DENIED)
                    e.Hints().emplace_back(TEXT("Please re-run with Administrator privilege."));
                ExceptionReport(e);
                return -1;
            }
        }

        ExceptionReport(Exception(__BASE_FILE__, __LINE__,
                                  TEXT("None of target files has been found."),
                                  TEXT("Are you sure the path you specified is correct?"),
                                  TEXT("The path you specified:"),
                                  argv[1]));
        return -1;
    } while (false);

    // -----------
    //  decide PatchSolutions
    // -----------

    if (MainExeMapper.IsValid()) {
        try {
            pSolution0.TakeOver(new PatchSolution0());
            pSolution0->SetFile(MainExeMapper->View<void>());
            if (pSolution0->FindPatchOffset() == false) {
                _putts(TEXT("[*] PatchSolution0 ...... Omitted"));
                pSolution0.Release();
            }
        } catch (Exception& e) {
            ExceptionReport(e);
            return -1;
        }
    } else {
        _putts(TEXT("[*] PatchSolution0 ...... Omitted"));
    }

    if (LibccDllMapper.IsValid()) {
        try {
            pSolution1.TakeOver(new PatchSolution1());
            pSolution2.TakeOver(new PatchSolution2());
            pSolution3.TakeOver(new PatchSolution3());

            pSolution1->SetFile(LibccDllMapper->View<void>());
            pSolution2->SetFile(LibccDllMapper->View<void>());
            pSolution3->SetFile(LibccDllMapper->View<void>());

            if (pSolution1->FindPatchOffset() == false) {
                _putts(TEXT("[*] PatchSolution1 ...... Omitted"));
                pSolution1.Release();
            }

            if (pSolution2->FindPatchOffset() == false) {
                _putts(TEXT("[*] PatchSolution2 ...... Omitted"));
                pSolution2.Release();
            }

            if (pSolution3->FindPatchOffset() == false) {
                _putts(TEXT("[*] PatchSolution3 ...... Omitted"));
                pSolution3.Release();
            }
        } catch (Exception& e) {
            ExceptionReport(e);
            return -1;
        }
    } else {
        _putts(TEXT("[*] PatchSolution1 ...... Omitted"));
        _putts(TEXT("[*] PatchSolution2 ...... Omitted"));
        _putts(TEXT("[*] PatchSolution3 ...... Omitted"));
    }

    if (pSolution0.IsValid() == false) {
        MainExeMapper.Release();
        MainExe.Release();
    }

    if (pSolution1.IsValid() == false && pSolution2.IsValid() == false && pSolution3.IsValid() == false) {
        LibccDllMapper.Release();
        LibccDll.Release();
    }

    if (pSolution0.IsValid() == false &&
        pSolution1.IsValid() == false &&
        pSolution2.IsValid() == false &&
        pSolution3.IsValid() == false) 
    {
        ExceptionReport(Exception(__BASE_FILE__, __LINE__,
                                  TEXT("Cannot find Navicat official RSA public key."),
                                  TEXT("Are you sure your Navicat has not been patched/modified before?")));
        return -1;
    }

    // -------------
    //  LoadKey
    // -------------
    try {
        LoadKey(pCipher,
                argc == 3 ? argv[2] : nullptr,
                pSolution0,
                pSolution1,
                pSolution2,
                pSolution3);
    } catch (Exception& e) {
        ExceptionReport(e);
        return -1;
    }

    // -------------
    //  BackupFile
    // -------------
    try {
        if (MainExe.IsValid())
            BackupFile(InstallationPath + MainExeName, 
                       InstallationPath + MainExeName + TEXT(".backup"));
    } catch (Exception& e) {
        if (e.HasErrorCode() && e.ErrorCode() == ERROR_FILE_EXISTS) {
            e.Hints().emplace_back(TStringFormat(TEXT("The backup of %s has been found."), 
                                                 MainExeName.c_str()));
            e.Hints().emplace_back(TStringFormat(TEXT("Please remove %s.backup in Navicat installation folder if you're sure %s has not been patched."),
                                                 MainExeName.c_str(),
                                                 MainExeName.c_str()));
            e.Hints().emplace_back(TStringFormat(TEXT("Otherwise please restore %s by %s.backup and remove %s.backup then try again."),
                                                 MainExeName.c_str(),
                                                 MainExeName.c_str(),
                                                 MainExeName.c_str()));
        }
        ExceptionReport(e);
        return -1;
    }

    try {
        if (LibccDll.IsValid())
            BackupFile(InstallationPath + LibccDllName, 
                       InstallationPath + LibccDllName + TEXT(".backup"));
    } catch (Exception& e) {
        if (e.HasErrorCode() && e.ErrorCode() == ERROR_FILE_EXISTS) {
            e.Hints().emplace_back(TStringFormat(TEXT("The backup of %s has been found."),
                                                 LibccDllName.c_str()));
            e.Hints().emplace_back(TStringFormat(TEXT("Please remove %s.backup in Navicat installation folder if you're sure %s has not been patched."),
                                                 LibccDllName.c_str(),
                                                 LibccDllName.c_str()));
            e.Hints().emplace_back(TStringFormat(TEXT("Otherwise please restore %s by %s.backup and remove %s.backup then try again."),
                                                 LibccDllName.c_str(),
                                                 LibccDllName.c_str(),
                                                 LibccDllName.c_str()));
        }
        ExceptionReport(e);
        return -1;
    }

    // -------------
    //  MakePatch
    // -------------
    try {
        if (pSolution0.IsValid()) {
            pSolution0->MakePatch(pCipher);
            _putts(TEXT(""));
        }
        if (pSolution1.IsValid()) {
            pSolution1->MakePatch(pCipher);
            _putts(TEXT(""));
        }
        if (pSolution2.IsValid()) {
            pSolution2->MakePatch(pCipher);
            _putts(TEXT(""));
        }
        if (pSolution3.IsValid()) {
            pSolution3->MakePatch(pCipher);
            _putts(TEXT(""));
        }
    } catch (Exception& e) {
        ExceptionReport(e);
        return -1;
    }

    _putts(TEXT("[*] Patch has been done successfully."));
    _putts(TEXT("[*] Have fun and enjoy~"));
    return 0;
}

