#pragma once
#include <openssl/des.h>
#include <random>
#include "TString.hpp"

class NavicatKeygen {
public:
    enum class Language {
        English,
        SimplifiedChinese,
        TraditionalChinese,
        Japanese,
        Polish,
        Spanish,
        French,
        German,
        Korean,
        Russian,
        Portuguese
    };

    enum class Product {
        DataModeler,
        Premium,
        MySQL,
        PostgreSQL,
        Oracle,
        SQLServer,
        SQLite,
        MariaDB,
        MongoDB,
        ReportViewer
    };
private:
    std::random_device _rand_dev;
    std::default_random_engine _rand_eng;
    std::uniform_int_distribution<int> _rand;
    uint8_t _data[10];

    void DoEncrypt() noexcept;
public:

    NavicatKeygen();

    void SetLanguageSignature(Language Value) noexcept;
    void SetLanguageSignature(uint8_t Value0, uint8_t Value1) noexcept;
    void SetProductSignature(Product Value) noexcept;
    void SetProductSignature(uint8_t Value) noexcept;
    void SetVersion(uint8_t VersionNumber) noexcept;
    void Generate() noexcept;
    TString GetSerialNumber(bool NoDash = false) const;
};
