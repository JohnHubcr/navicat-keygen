#include "NavicatKeygen.hpp"

void NavicatKeygen::DoEncrypt() noexcept {
    const_DES_cblock key = { 0x64, 0xAD, 0xF3, 0x2F, 0xAE, 0xF2, 0x1A, 0x27 };
    DES_key_schedule schedule;
    DES_set_key_unchecked(&key, &schedule);
    DES_ecb_encrypt(reinterpret_cast<const_DES_cblock*>(_data + 2),
                    reinterpret_cast<const_DES_cblock*>(_data + 2),
                    &schedule,
                    DES_ENCRYPT);
}

NavicatKeygen::NavicatKeygen() :
    _rand_eng(_rand_dev()),
    _rand(0, UINT8_MAX),
    _data{ 0x68 , 0x2A } {}

void NavicatKeygen::SetLanguageSignature(Language Value) noexcept {
    switch (Value) {
        case Language::English:
            _data[5] = 0xAC;       // Must be 0xAC for English version.
            _data[6] = 0x88;       // Must be 0x88 for English version.
            break;
        case Language::SimplifiedChinese:
            _data[5] = 0xCE;       // Must be 0xCE for Simplified Chinese version.
            _data[6] = 0x32;       // Must be 0x32 for Simplified Chinese version.
            break;
        case Language::TraditionalChinese:
            _data[5] = 0xAA;       // Must be 0xAA for Traditional Chinese version.
            _data[6] = 0x99;       // Must be 0x99 for Traditional Chinese version.
            break;
        case Language::Japanese:
            _data[5] = 0xAD;       // Must be 0xAD for Japanese version. Discoverer: @dragonflylee
            _data[6] = 0x82;       // Must be 0x82 for Japanese version. Discoverer: @dragonflylee
            break;
        case Language::Polish:
            _data[5] = 0xBB;       // Must be 0xBB for Polish version. Discoverer: @dragonflylee
            _data[6] = 0x55;       // Must be 0x55 for Polish version. Discoverer: @dragonflylee
            break;
        case Language::Spanish:
            _data[5] = 0xAE;       // Must be 0xAE for Spanish version. Discoverer: @dragonflylee
            _data[6] = 0x10;       // Must be 0x10 for Spanish version. Discoverer: @dragonflylee
            break;
        case Language::French:
            _data[5] = 0xFA;       // Must be 0xFA for French version. Discoverer: @Deltafox79
            _data[6] = 0x20;       // Must be 0x20 for French version. Discoverer: @Deltafox79
            break;
        case Language::German:
            _data[5] = 0xB1;       // Must be 0xB1 for German version. Discoverer: @dragonflylee
            _data[6] = 0x60;       // Must be 0x60 for German version. Discoverer: @dragonflylee
            break;
        case Language::Korean:
            _data[5] = 0xB5;       // Must be 0xB5 for Korean version. Discoverer: @dragonflylee
            _data[6] = 0x60;       // Must be 0x60 for Korean version. Discoverer: @dragonflylee
            break;
        case Language::Russian:
            _data[5] = 0xEE;       // Must be 0xB5 for Russian version. Discoverer: @dragonflylee
            _data[6] = 0x16;       // Must be 0x60 for Russian version. Discoverer: @dragonflylee
            break;
        case Language::Portuguese:
            _data[5] = 0xCD;       // Must be 0xCD for Portuguese version. Discoverer: @dragonflylee
            _data[6] = 0x49;       // Must be 0x49 for Portuguese version. Discoverer: @dragonflylee
            break;
        default:
            break;
    }
}

void NavicatKeygen::SetLanguageSignature(uint8_t Value0, uint8_t Value1) noexcept {
    _data[5] = Value0;
    _data[6] = Value1;
}

void NavicatKeygen::SetProductSignature(Product Value) noexcept {
    switch (Value) {
        case Product::DataModeler:
            _data[7] = 0x47;
            break;
        case Product::Premium:
            _data[7] = 0x65;
            break;
        case Product::MySQL:
            _data[7] = 0x68;
            break;
        case Product::PostgreSQL:
            _data[7] = 0x6C;
            break;
        case Product::Oracle:
            _data[7] = 0x70;
            break;
        case Product::SQLServer:
            _data[7] = 0x74;
            break;
        case Product::SQLite:
            _data[7] = 0x78;
            break;
        case Product::MariaDB:
            _data[7] = 0x7C;
            break;
        case Product::MongoDB:
            _data[7] = 0x80;
            break;
        case Product::ReportViewer:
            _data[7] = 0xb;
        default:
            break;
    }
}

void NavicatKeygen::SetProductSignature(uint8_t Value) noexcept {
    _data[7] = Value;
}

void NavicatKeygen::SetVersion(uint8_t VersionNumber) noexcept {
    _data[8] = VersionNumber << 4;
}

void NavicatKeygen::Generate() noexcept {
    _data[2] = _rand(_rand_eng);
    _data[3] = _rand(_rand_eng);
    _data[4] = _rand(_rand_eng);
    _data[9] = 0x32;
    DoEncrypt();
}

TString NavicatKeygen::GetSerialNumber(bool NoDash) const {
    static const TCHAR EncodeTable[] = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567");

    TString SerialNumber;
    SerialNumber.resize(16);

    SerialNumber[0] = EncodeTable[_data[0] >> 3];
    SerialNumber[1] = EncodeTable[(_data[0] & 0x07) << 2 | _data[1] >> 6];
    SerialNumber[2] = EncodeTable[_data[1] >> 1 & 0x1F];
    SerialNumber[3] = EncodeTable[(_data[1] & 0x1) << 4 | _data[2] >> 4];
    SerialNumber[4] = EncodeTable[(_data[2] & 0xF) << 1 | _data[3] >> 7];
    SerialNumber[5] = EncodeTable[_data[3] >> 2 & 0x1F];
    SerialNumber[6] = EncodeTable[_data[3] << 3 & 0x1F | _data[4] >> 5];
    SerialNumber[7] = EncodeTable[_data[4] & 0x1F];

    SerialNumber[8] = EncodeTable[_data[5] >> 3];
    SerialNumber[9] = EncodeTable[(_data[5] & 0x07) << 2 | _data[6] >> 6];
    SerialNumber[10] = EncodeTable[_data[6] >> 1 & 0x1F];
    SerialNumber[11] = EncodeTable[(_data[6] & 0x1) << 4 | _data[7] >> 4];
    SerialNumber[12] = EncodeTable[(_data[7] & 0xF) << 1 | _data[8] >> 7];
    SerialNumber[13] = EncodeTable[_data[8] >> 2 & 0x1F];
    SerialNumber[14] = EncodeTable[_data[8] << 3 & 0x1F | _data[9] >> 5];
    SerialNumber[15] = EncodeTable[_data[9] & 0x1F];

    if (NoDash) {
        return SerialNumber;
    } else {
        return TStringFormat(TEXT("%.4s-%.4s-%.4s-%.4s"),
                             SerialNumber.c_str(),
                             SerialNumber.c_str() + 4,
                             SerialNumber.c_str() + 8,
                             SerialNumber.c_str() + 12);
    }
}

