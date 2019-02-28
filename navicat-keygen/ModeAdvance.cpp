#include <ctime>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "ExceptionSystem.hpp"
#include "NavicatKeygen.hpp"
#include "RSACipher.hpp"
#include "Helper.hpp"

struct FileHandleTraits {
    using HandleType = HANDLE;
    static inline const HandleType InvalidValue = INVALID_HANDLE_VALUE;
    static constexpr auto& Releasor = CloseHandle;
};

void AdvanceMode(RSACipher* pCipher, bool bGenerateText) {
    int n;
    int n1, n2;
    NavicatKeygen keygen;
    TString username;
    TString organization;
    std::string utf8username;
    std::string utf8organization;

    char ResponseInfo[256] = {};
    std::vector<uint8_t> ResponseCode;

    rapidjson::Document json;
    rapidjson::Value N_Key;
    rapidjson::Value N_Value;
    rapidjson::Value O_Key;
    rapidjson::Value O_Value;
    rapidjson::Value T_Key;
    rapidjson::Value T_Value;
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    _putts(TEXT("[*] Navicat Product Signature:"));
    if (!Helper::ReadInt(n, 0, 255, 
                         TEXT("(range: 0x00 ~ 0xFF)> "), 
                         TEXT("Invalid number."))) {
        return;
    } else {
        keygen.SetProductSignature(n);
    }

    if ((_putts(TEXT("\n"
                     "[*] Navicat Language Signature 0:")), 
         Helper::ReadInt(n1, 0, 255,
                         TEXT("(range: 0x00 ~ 0xFF)> "),
                         TEXT("Invalid number."))) &&
        (_putts(TEXT("\n"
                     "[*] Navicat Language Signature 1:")), 
         Helper::ReadInt(n2, 0, 255,
                         TEXT("(range: 0x00 ~ 0xFF)> "),
                         TEXT("Invalid number.")))) {
        keygen.SetLanguageSignature(n1, n2);
    } else {
        return;
    }

    _putts(TEXT("\n"
                "[*] Input major version number:"));
    if (!Helper::ReadInt(n, 0, 16 - 1,
                         TEXT("(range: 0 ~ 15, default: 12)> "),
                         TEXT("Invalid number."))) {
        return;
    } else {
        keygen.SetVersion(n);
    }

    keygen.Generate();
    _tprintf_s(TEXT("\n"
                    "[*] Serial number:\n"
                    "%s\n"
                    "\n"), 
               keygen.GetSerialNumber().c_str());

    _tprintf_s(TEXT("[*] Your name: "));
    if (!std::getline(std::_tcin, username))
        return;
    _tprintf_s(TEXT("[*] Your organization: "));
    if (!std::getline(std::_tcin, organization))
        return;

    utf8username = TStringEncode(username, CP_UTF8);
    utf8organization = TStringEncode(organization, CP_UTF8);

    if (bGenerateText) {
        TString b64RequestCode;
        std::vector<uint8_t> RequestCode;
        char RequestInfo[256] = {};
        TString b64ResponseCode;

        _putts(TEXT("[*] Input request code in Base64: (Input empty line to end)"));
        while (true) {
            TString temp;
            if (!std::getline(std::_tcin, temp))
                return;

            if (temp.empty())
                break;

            b64RequestCode += temp;
        }

        RequestCode = Helper::Base64Decode(b64RequestCode);
        pCipher->Decrypt(RequestCode.data(), RequestCode.size(), RequestInfo, RSA_PKCS1_PADDING);

        printf_s("[*]Request Info:\n"
                 "%s\n"
                 "\n", 
                 RequestInfo);

        json.Parse(RequestInfo);

        //
        // Remove "Platform" info
        //
        json.RemoveMember("P");
        //
        // Set "Name" info
        //
        N_Key.SetString("N", 1);
        N_Value.SetString(utf8username.c_str(), utf8username.length());
        //
        // Set "Organization" info
        //
        O_Key.SetString("O", 1);
        O_Value.SetString(utf8organization.c_str(), utf8organization.length());
        //
        // Set "Time" info
        //
        T_Key.SetString("T", 1);
        T_Value.SetUint(std::time(nullptr));
        //
        // Add "Name", "Organization" and "Time"
        //
        json.AddMember(N_Key, N_Value, json.GetAllocator());
        json.AddMember(O_Key, O_Value, json.GetAllocator());
        json.AddMember(T_Key, T_Value, json.GetAllocator());

        json.Accept(writer);
        if (buffer.GetSize() > 240)
            throw Exception(__BASE_FILE__, __LINE__,
                            TEXT("Response info is too long."));

        memcpy(ResponseInfo, buffer.GetString(), buffer.GetSize());

        _tprintf_s(TEXT("[*] Response Info:\n"
                        "%s\n"
                        "\n"), 
                   TStringBuilder(ResponseInfo, CP_UTF8).c_str());

        ResponseCode.resize(256);
        pCipher->Encrypt<RSAKeyType::PrivateKey>(ResponseInfo,
                                                 strlen(ResponseInfo),
                                                 ResponseCode.data(),
                                                 RSA_PKCS1_PADDING);
        b64ResponseCode = Helper::Base64Encode(ResponseCode);

        _tprintf_s(TEXT("[*] Activation Code:\n"
                        "%s\n"
                        "\n"),
                   b64ResponseCode.c_str());
    } else {    // Generate license_file
        ResourceObject<FileHandleTraits> hLicenseFile;
        rapidjson::Value K_Key;
        rapidjson::Value K_Value;
        std::string utf8SerialNumber = TStringEncode(keygen.GetSerialNumber(true), CP_UTF8);
        DWORD NumberOfBytesWritten;

        json.Parse("{}");

        K_Key.SetString("K", 1);
        K_Value.SetString(utf8SerialNumber.c_str(), utf8SerialNumber.length());
        N_Key.SetString("N", 1);
        N_Value.SetString(utf8username.c_str(), utf8username.length());
        O_Key.SetString("O", 1);
        O_Value.SetString(utf8organization.c_str(), utf8organization.length());
        T_Key.SetString("T", 1);
        T_Value.SetUint(std::time(nullptr));

        json.AddMember(K_Key, K_Value, json.GetAllocator());
        json.AddMember(N_Key, N_Value, json.GetAllocator());
        json.AddMember(O_Key, O_Value, json.GetAllocator());
        json.AddMember(T_Key, T_Value, json.GetAllocator());

        json.Accept(writer);
        if (buffer.GetSize() > 240)
            throw Exception(__BASE_FILE__, __LINE__,
                            TEXT("Response info is too long."));

        memcpy(ResponseInfo, buffer.GetString(), buffer.GetSize());

        _tprintf_s(TEXT("\n"
                        "[*] Response Info:\n"
                        "%s\n"
                        "\n"),
                   TStringBuilder(ResponseInfo, CP_UTF8).c_str());

        ResponseCode.resize(256);
        pCipher->Encrypt<RSAKeyType::PrivateKey>(ResponseInfo,
                                                 strlen(ResponseInfo),
                                                 ResponseCode.data(),
                                                 RSA_PKCS1_PADDING);

        hLicenseFile.TakeOver(CreateFile(TEXT("license_file"),
                                         GENERIC_READ | GENERIC_WRITE,
                                         NULL,
                                         NULL,
                                         CREATE_ALWAYS,
                                         FILE_ATTRIBUTE_NORMAL,
                                         NULL));
        if (hLicenseFile.IsValid() == false)
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("CreateFile failed."));

        if (!WriteFile(hLicenseFile, ResponseCode.data(), ResponseCode.size(), &NumberOfBytesWritten, NULL))
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("WriteFile failed."));

        _putts(TEXT("[*] license_file has been generated."));
    }
}

