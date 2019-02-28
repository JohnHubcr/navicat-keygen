#pragma once
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <memory.h>
#include "Exception.hpp"
#include "ResourceObject.hpp"

#ifdef _DEBUG
#pragma comment(lib, "libcryptoMTd.lib")
#else
#pragma comment(lib, "libcryptoMT.lib")
#endif
#pragma comment(lib, "WS2_32.lib")      // some symbol are used in OpenSSL static lib
#pragma comment(lib, "Crypt32.lib")     // some symbol are used in OpenSSL static lib

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("RSACipher.hpp")

class OpensslException : public Exception {
private:
    unsigned long _ErrorCode;
    TString _ErrorString;
public:

    OpensslException(const TString& FileName, 
                     size_t LineNumber, 
                     unsigned long OpensslErrorCode, 
                     const TString& CustomMsg) noexcept :
        Exception(FileName, LineNumber, CustomMsg),
        _ErrorCode(OpensslErrorCode),
        _ErrorString(TStringBuilder(ERR_error_string(OpensslErrorCode, nullptr), CP_UTF8)) {}

    virtual bool HasErrorCode() const noexcept override {
        return true;
    }

    virtual uintptr_t ErrorCode() const noexcept override {
        return _ErrorCode;
    }

    virtual const TString& ErrorString() const noexcept override {
        return _ErrorString;
    }

};

struct OpensslBIOTraits {
    using HandleType = BIO*;
    static inline const HandleType InvalidValue = nullptr;
    static constexpr auto& Releasor = BIO_free;
};

struct OpensslBIOChainTraits {
    using HandleType = BIO*;
    static inline const HandleType InvalidValue = nullptr;
    static constexpr auto& Releasor = BIO_free_all;
};

struct OpensslBNTraits {
    using HandleType = BIGNUM*;
    static inline const HandleType InvalidValue = nullptr;
    static constexpr auto& Releasor = BN_free;
};

struct OpensslRSATraits {
    using HandleType = RSA*;
    static inline const HandleType InvalidValue = nullptr;
    static constexpr auto& Releasor = RSA_free;
};

enum class RSAKeyType {
    PrivateKey,
    PublicKey
};

enum class RSAKeyFormat {
    NotSpecified,
    PEM,
    PKCS1
};

class RSACipher {
private:
    ResourceObject<OpensslRSATraits> _RSAObj;

    RSACipher(RSA* pRsa) noexcept : _RSAObj(pRsa) {}

    //
    // Copy constructor is not allowed
    //
    RSACipher(const RSACipher&) = delete;

    //
    // Copy assignment is not allowed
    //
    RSACipher& operator=(const RSACipher&) = delete;

    template<RSAKeyType __Type, RSAKeyFormat __Format = RSAKeyFormat::NotSpecified>
    static void _WriteRSAToBIO(RSA* PtrToRSA, BIO* PtrToBIO) {
        if constexpr (__Type == RSAKeyType::PrivateKey) {
            if (!PEM_write_bio_RSAPrivateKey(PtrToBIO, PtrToRSA, nullptr, nullptr, 0, nullptr, nullptr))
                throw Exception(__BASE_FILE__, __LINE__, 
                                TEXT("PEM_write_bio_RSAPrivateKey failed."));
        } else {
            if constexpr (__Format == RSAKeyFormat::PEM) {
                if (!PEM_write_bio_RSA_PUBKEY(PtrToBIO, PtrToRSA))
                    throw Exception(__BASE_FILE__, __LINE__, 
                                    TEXT("PEM_write_bio_RSA_PUBKEY failed."));
            } else if constexpr (__Format == RSAKeyFormat::PKCS1) {
                if (!PEM_write_bio_RSAPublicKey(PtrToBIO, PtrToRSA))
                    throw Exception(__BASE_FILE__, __LINE__, 
                                    TEXT("PEM_write_bio_RSAPublicKey failed."));
            } else {
                static_assert(__Format == RSAKeyFormat::PEM || __Format == RSAKeyFormat::PKCS1);
            }
        }
    }

    template<RSAKeyType __Type, RSAKeyFormat __Format = RSAKeyFormat::NotSpecified>
    static RSA* _ReadRSAFromBIO(BIO* PtrToBIO) {
        RSA* PtrToRSA;

        if constexpr (__Type == RSAKeyType::PrivateKey) {
            PtrToRSA = PEM_read_bio_RSAPrivateKey(PtrToBIO, nullptr, nullptr, nullptr);
            if (PtrToRSA == nullptr)
                throw Exception(__BASE_FILE__, __LINE__, 
                                TEXT("PEM_read_bio_RSAPrivateKey failed."));
        } else {
            if constexpr (__Format == RSAKeyFormat::PEM) {
                PtrToRSA = PEM_read_bio_RSA_PUBKEY(PtrToBIO, nullptr, nullptr, nullptr);
                if (PtrToRSA == nullptr)
                    throw Exception(__BASE_FILE__, __LINE__, 
                                    TEXT("PEM_read_bio_RSA_PUBKEY failed."));
            } else if constexpr (__Format == RSAKeyFormat::PKCS1) {
                PtrToRSA = PEM_read_bio_RSAPublicKey(PtrToBIO, nullptr, nullptr, nullptr);
                if (PtrToRSA == nullptr)
                    throw Exception(__BASE_FILE__, __LINE__, 
                                    TEXT("PEM_read_bio_RSAPublicKey failed."));
            } else {
                static_assert(__Format == RSAKeyFormat::PEM || __Format == RSAKeyFormat::PKCS1);
            }
        }

        return PtrToRSA;
    }

public:

    RSACipher() : _RSAObj(RSA_new()) {
        if (_RSAObj.IsValid() == false)
            throw OpensslException(__BASE_FILE__, __LINE__, ERR_get_error(), 
                                   TEXT("RSA_new failed."));
    }

    void GenerateKey(int bits, unsigned int e = RSA_F4) {
        ResourceObject<OpensslBNTraits> bn_e;

        bn_e.TakeOver(BN_new());
        if (bn_e.IsValid() == false)
            throw OpensslException(__BASE_FILE__, __LINE__, ERR_get_error(),
                                   TEXT("BN_new failed."));

        if (!BN_set_word(bn_e, e))
            throw Exception(__BASE_FILE__, __LINE__,
                            TEXT("BN_set_word failed."));

        if (!RSA_generate_key_ex(_RSAObj, bits, bn_e, nullptr))
            throw OpensslException(__BASE_FILE__, __LINE__, ERR_get_error(),
                                   TEXT("RSA_generate_key_ex failed."));
    }

    template<RSAKeyType __Type, RSAKeyFormat __Format = RSAKeyFormat::NotSpecified>
    void ExportKeyToFile(const std::string& FileName) {
        ResourceObject<OpensslBIOTraits> BIOKeyFile;

        BIOKeyFile.TakeOver(BIO_new_file(FileName.c_str(), "w"));
        if (BIOKeyFile.IsValid() == false)
            throw Exception(__BASE_FILE__, __LINE__,
                            TEXT("BIO_new_file failed."));

        _WriteRSAToBIO<__Type, __Format>(_RSAObj, BIOKeyFile);
    }

    template<RSAKeyType __Type, RSAKeyFormat __Format = RSAKeyFormat::NotSpecified>
    std::string ExportKeyString() {
        std::string KeyString;
        ResourceObject<OpensslBIOTraits> BIOKeyMemory;
        long s;
        const char* p = nullptr;

        BIOKeyMemory.TakeOver(BIO_new(BIO_s_mem()));
        if (BIOKeyMemory.IsValid() == false)
            throw Exception(__BASE_FILE__, __LINE__,
                            TEXT("BIO_new failed."));

        _WriteRSAToBIO<__Type, __Format>(_RSAObj, BIOKeyMemory);

        s = BIO_get_mem_data(BIOKeyMemory, &p);
        KeyString.resize(s);
        memcpy(KeyString.data(), p, s);

        return KeyString;
    }

    template<RSAKeyType __Type, RSAKeyFormat __Format = RSAKeyFormat::NotSpecified>
    void ImportKeyFromFile(const std::string& FileName) {
        ResourceObject<OpensslBIOTraits> BIOKeyFile;
        RSA* NewRSAObj;

        BIOKeyFile.TakeOver(BIO_new_file(FileName.c_str(), "r"));
        if (BIOKeyFile.IsValid() == false)
            throw Exception(__BASE_FILE__, __LINE__,
                            TEXT("BIO_new_file failed."));

        NewRSAObj = _ReadRSAFromBIO<__Type, __Format>(BIOKeyFile);
        _RSAObj.Release();
        _RSAObj.TakeOver(NewRSAObj);
    }

    template<RSAKeyType __Type, RSAKeyFormat __Format = RSAKeyFormat::NotSpecified>
    void ImportKeyString(const std::string& KeyString) {
        ResourceObject<OpensslBIOTraits> BIOKeyMemory;
        RSA* NewRSAObj;

        BIOKeyMemory = BIO_new(BIO_s_mem());
        if (BIOKeyMemory == nullptr)
            throw Exception(__BASE_FILE__, __LINE__,
                            TEXT("BIO_new failed."));

        if (BIO_puts(BIOKeyMemory, KeyString.c_str()) <= 0)
            throw Exception(__BASE_FILE__, __LINE__,
                            TEXT("BIO_puts failed."));

        NewRSAObj = _ReadRSAFromBIO<__Type, __Format>(BIOKeyMemory);
        _RSAObj.Release();
        _RSAObj.TakeOver(NewRSAObj);
    }

    template<RSAKeyType __Type = RSAKeyType::PublicKey>
    int Encrypt(const void* from, int len, void* to, int padding) {
        int WriteBytes;

        if constexpr (__Type == RSAKeyType::PrivateKey) {
            WriteBytes = RSA_private_encrypt(len,
                                             reinterpret_cast<const unsigned char*>(from),
                                             reinterpret_cast<unsigned char*>(to),
                                             _RSAObj,
                                             padding);
            if (WriteBytes == -1)
                throw OpensslException(__BASE_FILE__, __LINE__, ERR_get_error(),
                                       TEXT("RSA_private_encrypt failed."));
        } else {
            WriteBytes = RSA_public_encrypt(len,
                                            reinterpret_cast<const unsigned char*>(from),
                                            reinterpret_cast<unsigned char*>(to),
                                            _RSAObj,
                                            padding);
            if (WriteBytes == -1)
                throw OpensslException(__BASE_FILE__, __LINE__, ERR_get_error(),
                                       TEXT("RSA_public_encrypt failed."));
        }

        return WriteBytes;
    }

    template<RSAKeyType __Type = RSAKeyType::PrivateKey>
    int Decrypt(const void* from, int len, void* to, int padding) {
        int WriteBytes;

        if constexpr (__Type == RSAKeyType::PrivateKey) {
            WriteBytes = RSA_private_decrypt(len,
                                             reinterpret_cast<const unsigned char*>(from),
                                             reinterpret_cast<unsigned char*>(to),
                                             _RSAObj,
                                             padding);
            if (WriteBytes == -1)
                throw OpensslException(__BASE_FILE__, __LINE__, ERR_get_error(),
                                       TEXT("RSA_private_decrypt failed."));
        } else {
            WriteBytes = RSA_public_decrypt(len,
                                            reinterpret_cast<const unsigned char*>(from),
                                            reinterpret_cast<unsigned char*>(to),
                                            _RSAObj,
                                            padding);
            if (WriteBytes == -1)
                throw OpensslException(__BASE_FILE__, __LINE__, ERR_get_error(),
                                       TEXT("RSA_public_decrypt failed."));
        }

        return WriteBytes;
    }

};


