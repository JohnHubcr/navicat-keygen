#pragma once
#include "RSACipher.hpp"
#include "CapstoneDisassembler.hpp"
#include "ImageInterpreter.hpp"

class PatchSolution{
public:
    virtual void SetFile(void* pFile) = 0;
    virtual bool FindPatchOffset() noexcept = 0;
    virtual bool CheckKey(RSACipher* pCipher) const = 0;
    virtual void MakePatch(RSACipher* pCipher) const = 0;
    virtual ~PatchSolution() {}
};

// PatchSolution0 will replace the RSA public key stored in main application.
// Main application can be "Navicat.exe", "Modeler.exe" or "Rviewer.exe"
class PatchSolution0 : public PatchSolution {
private:
    static const char Keyword[461];

    ImageInterpreter _MainExeInterpreter;
    off_t _PatchOffset;
public:
    PatchSolution0();
    virtual void SetFile(void* pFile) override;
    virtual bool FindPatchOffset() noexcept override;
    virtual bool CheckKey(RSACipher* pCipher) const override;
    virtual void MakePatch(RSACipher* cipher) const override;
};

// PatchSolution1, 2, 3 will replace the RSA public key stored in libcc.dll
class PatchSolution1 : public PatchSolution {
private:

    enum KeywordDataType {
        IMM_DATA,
        STRING_DATA
    };

    struct KeywordInfo {
        const char* Data;
        size_t Length;
        KeywordDataType Type;
    };

    struct PatchPointInfo {
        uint8_t* Ptr;
        size_t PatchSize;
        size_t MaxPatchSize;
    };

    static const KeywordInfo Keywords[5];

    ImageInterpreter _LibccDllInterpreter;
    mutable PatchPointInfo _Patches[5];
public:
    PatchSolution1();
    virtual void SetFile(void* pFile) override;
    virtual bool FindPatchOffset() noexcept override;
    virtual bool CheckKey(RSACipher* pCipher) const override;
    virtual void MakePatch(RSACipher* pCipher) const override;
};

class PatchSolution2 : public PatchSolution {
private:
    static const char KeywordsMeta[0x188 + 1];
    static uint8_t Keywords[0x188][5];

    ImageInterpreter _LibccDllInterpreter;
    off_t _PatchOffsets[0x188];

    void BuildKeywords() noexcept;
public:
    PatchSolution2();
    virtual void SetFile(void* pFile) override;
    virtual bool FindPatchOffset() noexcept override;
    virtual bool CheckKey(RSACipher* pCipher) const override;
    virtual void MakePatch(RSACipher* pCipher) const override;
};

class PatchSolution3 : public PatchSolution {
private:
    enum KeywordDataType {
        IMM_DATA,
        STRING_DATA,
    };

    struct KeywordInfo {
        uint8_t Data[8];
        size_t Length;
        KeywordDataType Type;
        bool NotRecommendedToModify;
    };

    struct PatchPointInfo {
        union {
            uint8_t* Ptr;
            const uint8_t* ConstPtr;
        } Opcode;
        uint64_t OpcodeRva;
        uint8_t* PatchPtr;
        size_t PatchSize;
        char* OriginalStringPtr;
        char* ReplaceStringPtr;
    };

    static const KeywordInfo Keywords[111];

    CapstoneEngine _CapstoneEngine;
    ImageInterpreter _LibccDllInterpreter;
    mutable PatchPointInfo _Patches[111];

    bool CheckIfMatchPattern(cs_insn* pInsn) const;

    bool CheckIfFound(cs_insn* pInsn, size_t i) const;

    PatchPointInfo 
    CreatePatchPoint(const uint8_t* pOpcode, cs_insn* pInsn, size_t i) const;

    CapstoneDisassembler::Context 
    GetJumpedBranch(const CapstoneDisassembler::Context& NotJumpedBranch, cs_insn* pJxxInsn) const;

    CapstoneDisassembler::Context 
    HandleJcc(const CapstoneDisassembler::Context& NotJumpedBranch,
              const CapstoneDisassembler::Context& JumpedBranch,
              size_t i) const;
public:
    PatchSolution3();
    virtual void SetFile(void* pLibccFile) override;
    virtual bool FindPatchOffset() noexcept override;
    virtual bool CheckKey(RSACipher* pCipher) const override;
    virtual void MakePatch(RSACipher* pCipher) const override;
};


