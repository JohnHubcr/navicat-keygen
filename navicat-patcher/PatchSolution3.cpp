#include "PatchSolution.hpp"
#include "Exception.hpp"
#include "Helper.hpp"
#include <tchar.h>

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("PatchSolution3.cpp")

const PatchSolution3::KeywordInfo PatchSolution3::Keywords[111] = {
    { { 0x4d, 0x49, 0x49 }, 3, STRING_DATA, false },
    { { 0x42, 0x49 }, 2, IMM_DATA, false },
    { { 0x6a }, 1, IMM_DATA, false },
    { { 0x41 }, 1, IMM_DATA, false },
    { { 0x4e, 0x42, 0x67, 0x6b }, 4, IMM_DATA, false },
    { { 0x71 }, 1, IMM_DATA, false },
    { { 0x68, 0x6b, 0x69, 0x47, 0x39, 0x77 }, 6, STRING_DATA, false },
    { { 0x30 }, 1, STRING_DATA, true },
    { { 0x42 }, 1, IMM_DATA, false },
    { { 0x41 }, 1, IMM_DATA, false },
    { { 0x51, 0x45, 0x46, 0x41, 0x41, 0x4f, 0x43 }, 7, STRING_DATA, false },
    { { 0x41, 0x51, 0x38, 0x41, 0x4d, 0x49 }, 6, STRING_DATA, false },
    { { 0x49, 0x42 }, 2, STRING_DATA, false },
    { { 0x43, 0x67, 0x4b, 0x43 }, 4, IMM_DATA, false },
    { { 0x41, 0x51 }, 2, STRING_DATA, false },
    { { 0x45, 0x41, 0x77, 0x31 }, 4, IMM_DATA, false },
    { { 0x64, 0x71, 0x46, 0x33 }, 4, IMM_DATA, false },
    { { 0x53 }, 1, STRING_DATA, true },
    { { 0x6b, 0x43, 0x61, 0x41, 0x41, 0x6d }, 6, STRING_DATA, false },
    { { 0x4d, 0x7a, 0x73, 0x38 }, 4, IMM_DATA, false },
    { { 0x38, 0x39, 0x49, 0x71 }, 4, IMM_DATA, false },
    { { 0x64 }, 1, IMM_DATA, false },
    { { 0x57 }, 1, IMM_DATA, false },
    { { 0x39, 0x4d, 0x32, 0x64 }, 4, IMM_DATA, false },
    { { 0x49, 0x64, 0x68 }, 3, STRING_DATA, false },
    { { 0x33, 0x6a }, 2, IMM_DATA, false },
    { { 0x47, 0x39, 0x79, 0x50 }, 4, IMM_DATA, false },
    { { 0x63, 0x6d }, 2, IMM_DATA, false },
    { { 0x4c }, 1, IMM_DATA, false },
    { { 0x6e, 0x6d, 0x4a }, 3, STRING_DATA, false },
    { { 0x69, 0x47, 0x70, 0x42, 0x46, 0x34, 0x45 }, 7, STRING_DATA, false },
    { { 0x39, 0x56, 0x48, 0x53, 0x4d, 0x47 }, 6, STRING_DATA, false },
    { { 0x65, 0x38, 0x6f, 0x50, 0x41, 0x79, 0x32, 0x6b }, 8, STRING_DATA, false },
    { { 0x4a, 0x44 }, 2, STRING_DATA, false },
    { { 0x6d, 0x64 }, 2, IMM_DATA, false },
    { { 0x4e, 0x74, 0x34 }, 3, STRING_DATA, false },
    { { 0x42, 0x63, 0x45, 0x79, 0x67, 0x76 }, 6, STRING_DATA, false },
    { { 0x73, 0x73, 0x45, 0x66, 0x67, 0x69 }, 6, STRING_DATA, false },
    { { 0x6e, 0x76, 0x61, 0x35, 0x74 }, 5, STRING_DATA, false },
    { { 0x35, 0x6a, 0x6d, 0x33, 0x35, 0x32 }, 6, STRING_DATA, false },
    { { 0x55, 0x41 }, 2, IMM_DATA, false },
    { { 0x6f, 0x44, 0x6f, 0x73 }, 4, IMM_DATA, false },
    { { 0x55, 0x4a }, 2, IMM_DATA, false },
    { { 0x6b, 0x54, 0x58, 0x47, 0x51 }, 5, STRING_DATA, false },
    { { 0x68, 0x70, 0x41, 0x57, 0x4d, 0x46 }, 6, STRING_DATA, false },
    { { 0x34, 0x66, 0x42, 0x6d, 0x42 }, 5, STRING_DATA, false },
    { { 0x70, 0x4f, 0x33, 0x45 }, 4, IMM_DATA, false },
    { { 0x65, 0x64 }, 2, IMM_DATA, false },
    { { 0x47 }, 1, IMM_DATA, false },
    { { 0x36, 0x32, 0x72, 0x4f, 0x73, 0x71 }, 6, STRING_DATA, false },
    { { 0x4d }, 1, IMM_DATA, false },
    { { 0x42, 0x67, 0x6d, 0x53 }, 4, STRING_DATA, false },
    { { 0x64 }, 1, IMM_DATA, false },
    { { 0x41, 0x79, 0x78, 0x43, 0x53 }, 5, STRING_DATA, false },
    { { 0x50 }, 1, IMM_DATA, false },
    { { 0x42, 0x52, 0x4a, 0x49, 0x4f }, 5, STRING_DATA, false },
    { { 0x46, 0x52, 0x30, 0x51, 0x67, 0x5a, 0x46, 0x62 }, 8, STRING_DATA, false },
    { { 0x52 }, 1, IMM_DATA, false },
    { { 0x6e, 0x55, 0x30, 0x66 }, 4, STRING_DATA, false },
    { { 0x72, 0x6a, 0x33, 0x34 }, 4, IMM_DATA, false },
    { { 0x66 }, 1, STRING_DATA, true },
    { { 0x69, 0x56, 0x6d, 0x67 }, 4, IMM_DATA, false },
    { { 0x59, 0x69, 0x4c, 0x75 }, 4, STRING_DATA, false },
    { { 0x5a, 0x53, 0x41, 0x6d }, 4, IMM_DATA, false },
    { { 0x49, 0x62 }, 2, IMM_DATA, false },
    { { 0x73 }, 1, IMM_DATA, false },
    { { 0x38, 0x5a, 0x78, 0x69 }, 4, IMM_DATA, false },
    { { 0x48 }, 1, IMM_DATA, false },
    { { 0x50, 0x64, 0x70, 0x31 }, 4, IMM_DATA, false },
    { { 0x6f, 0x44 }, 2, IMM_DATA, false },
    { { 0x34 }, 1, IMM_DATA, false },
    { { 0x74, 0x55, 0x70, 0x76, 0x73, 0x46 }, 6, STRING_DATA, false },
    { { 0x63, 0x69, 0x34, 0x51, 0x4a, 0x74 }, 6, STRING_DATA, false },
    { { 0x59, 0x4e, 0x6a, 0x4e, 0x6e, 0x47, 0x55 }, 7, STRING_DATA, false },
    { { 0x32, 0x57, 0x50, 0x48 }, 4, STRING_DATA, false },
    { { 0x36, 0x72, 0x76, 0x43, 0x68, 0x47, 0x6c }, 7, STRING_DATA, false },
    { { 0x31, 0x49, 0x52, 0x4b, 0x72, 0x78, 0x4d, 0x74 }, 8, STRING_DATA, false },
    { { 0x71, 0x4c, 0x69, 0x65, 0x6c }, 5, STRING_DATA, false },
    { { 0x73, 0x76, 0x61, 0x6a, 0x55, 0x6a, 0x79, 0x72 }, 8, STRING_DATA, false },
    { { 0x67 }, 1, STRING_DATA, true },
    { { 0x4f, 0x43, 0x36, 0x4e, 0x6d, 0x79, 0x6d, 0x59 }, 8, STRING_DATA, false },
    { { 0x4d }, 1, IMM_DATA, false },
    { { 0x76, 0x5a, 0x4e }, 3, STRING_DATA, false },
    { { 0x45, 0x52, 0x33, 0x68, 0x74 }, 5, STRING_DATA, false },
    { { 0x46 }, 1, IMM_DATA, false },
    { { 0x45, 0x74, 0x4c, 0x31 }, 4, STRING_DATA, false },
    { { 0x65, 0x51, 0x62, 0x43, 0x79 }, 5, STRING_DATA, false },
    { { 0x54, 0x66, 0x44, 0x6d, 0x74, 0x59, 0x79, 0x51 }, 8, STRING_DATA, false },
    { { 0x31, 0x57, 0x74, 0x34 }, 4, STRING_DATA, false },
    { { 0x4f }, 1, IMM_DATA, false },
    { { 0x74, 0x31, 0x32, 0x6c, 0x78, 0x66 }, 6, STRING_DATA, false },
    { { 0x30 }, 1, IMM_DATA, false },
    { { 0x77, 0x56, 0x49, 0x52, 0x35 }, 5, STRING_DATA, false },
    { { 0x6d }, 1, IMM_DATA, false },
    { { 0x63, 0x47, 0x4e, 0x37 }, 4, STRING_DATA, false },
    { { 0x58, 0x43, 0x58, 0x4a }, 4, STRING_DATA, false },
    { { 0x52, 0x48, 0x4f, 0x46 }, 4, IMM_DATA, false },
    { { 0x48, 0x53 }, 2, IMM_DATA, false },
    { { 0x66 }, 1, IMM_DATA, false },
    { { 0x31, 0x67, 0x7a, 0x58, 0x57 }, 5, STRING_DATA, false },
    { { 0x61 }, 1, IMM_DATA, false },
    { { 0x62 }, 1, IMM_DATA, false },
    { { 0x52, 0x53 }, 2, STRING_DATA, false },
    { { 0x76, 0x6d, 0x74, 0x31, 0x6e }, 5, STRING_DATA, false },
    { { 0x72, 0x6c }, 2, STRING_DATA, true },
    { { 0x37, 0x73, 0x57 }, 3, STRING_DATA, false },
    { { 0x36, 0x63, 0x6a }, 3, STRING_DATA, false },
    { { 0x78, 0x6c, 0x6a, 0x75, 0x75, 0x51, 0x61 }, 7, STRING_DATA, false },
    { { 0x77, 0x49, 0x44, 0x41 }, 4, STRING_DATA, false },
    { { 0x51, 0x41 }, 2, IMM_DATA, false },
    { { 0x42 }, 1, IMM_DATA, false },
};

#if defined(_M_AMD64)
bool PatchSolution3::CheckIfMatchPattern(cs_insn* pInsn) const {
    // the instruction we're interested in has one of the following patterns:
    //  1. mov PTR [MEM], IMM   (IMM must consist of printable chars)               // for IMM_DATA
    //  2. lea REG, PTR [MEM]   (MEM must point to a non-empty printable string)    // for STRING_DATA

    if (_stricmp(pInsn->mnemonic, "mov") == 0) {
        if (pInsn->detail->x86.operands[1].type != X86_OP_IMM)
            return false;

        uint8_t* p_imm = pInsn->bytes + pInsn->detail->x86.encoding.imm_offset;
        uint8_t s_imm = pInsn->detail->x86.encoding.imm_size;

        return Helper::IsPrintable(p_imm, s_imm);
    } else if (_stricmp(pInsn->mnemonic, "lea") == 0) {
        // as far as I know, all strings are loaded by "lea REG, QWORD PTR[RIP + disp]"
        // so operands[1] must look like "[RIP + disp]"
        if (pInsn->detail->x86.operands[1].mem.base != X86_REG_RIP)
            return false;

        // scale must 1, otherwise pattern mismatches
        if (pInsn->detail->x86.operands[1].mem.scale != 1)
            return false;

        auto StringRva = static_cast<uintptr_t>(
            pInsn->address + pInsn->size +    // RIP
            pInsn->detail->x86.operands[1].mem.disp
        );

        auto StringPtr = _LibccDllInterpreter.RvaToPointer<uint8_t>(StringRva);

        // If not found, pattern mismatches
        if (StringPtr == nullptr)
            return false;

        // StringPtr must have at least one char
        // every char in StringPtr must be printable, otherwise pattern mismatches
        auto StringLength = strlen(reinterpret_cast<char*>(StringPtr));
        if (StringLength && Helper::IsPrintable(StringPtr, StringLength))
            return true;

        return false;
    } else {
        return false;
    }
}
#else       // for i386 
bool PatchSolution3::CheckIfMatchPattern(cs_insn* pInsn) const {
    // the instruction we're interested in has one of the following patterns:
    //  1. mov PTR [MEM], IMM   (IMM must consist of printable chars)               // for KeywordType::IMM_DATA
    //     except pattern "mov [ebp - 0x4], IMM"
    //  2. push IMM             (IMM must consist of printable chars)               // for KeywordType::IMM_DATA
    //  3. push offset MEM      (MEM must point to a non-empty printable string)    // for KeywordType::STRING_DATA                     
    //

    if (_stricmp(pInsn->mnemonic, "mov") == 0) {
        // filter the case "mov [ebp - 0x4], IMM"
        // because IMM may consist of printable chars in that case, which can mislead us.
        //
        // Here I use "> -0x30" to intensify condition, instead of "== -0x4"
        if (pInsn->detail->x86.operands[0].type == X86_OP_MEM &&
            pInsn->detail->x86.operands[0].mem.base == X86_REG_EBP &&
            pInsn->detail->x86.operands[0].mem.disp > -0x30)
            return false;

        if (pInsn->detail->x86.operands[1].type != X86_OP_IMM)
            return false;

        uint8_t* p_imm = pInsn->bytes + pInsn->detail->x86.encoding.imm_offset;
        uint8_t s_imm = pInsn->detail->x86.encoding.imm_size;

        // each bytes of imm must be printable;
        return Helper::IsPrintable(p_imm, s_imm);
    } else if (_stricmp(pInsn->mnemonic, "push") == 0) {
        if (pInsn->detail->x86.operands[0].type != X86_OP_IMM)
            return false;

        {   // test if match pattern 2
            if (Helper::IsPrintable(pInsn->bytes + pInsn->detail->x86.encoding.imm_offset,
                                    pInsn->detail->x86.encoding.imm_size))
                return true;
        }

        {   // test if match pattern 3
            auto StringRva = static_cast<uintptr_t>(
                pInsn->detail->x86.operands[0].imm -
                _LibccDllInterpreter.GetImageNTHeaders()->OptionalHeader.ImageBase
            );

            auto StringPtr = _LibccDllInterpreter.RvaToPointer<uint8_t>(StringRva);

            if (StringPtr) {
                auto StringLength = strlen(reinterpret_cast<char*>(StringPtr));

                if (StringLength && Helper::IsPrintable(StringPtr, StringLength))
                    return true;
            }
        }

        return false;
    } else {
        return false;
    }
}
#endif

#if defined(_M_AMD64)
bool PatchSolution3::CheckIfFound(cs_insn* pInsn, size_t i) const {
    auto& op_count = pInsn->detail->x86.op_count;
    auto& operands = pInsn->detail->x86.operands;

    if (pInsn->detail->x86.op_count != 2)
        return false;
    
    if (Keywords[i].Type == IMM_DATA && operands[1].type == X86_OP_IMM) {
        static_assert(sizeof(operands[1].imm) == sizeof(Keywords[i].Data));
        return 
            operands[1].imm == *reinterpret_cast<const int64_t*>(Keywords[i].Data) &&
            pInsn->detail->x86.encoding.imm_size == Keywords[i].Length;
    } else if (Keywords[i].Type == STRING_DATA && operands[1].type == X86_OP_MEM) {
        auto StringRva = static_cast<uintptr_t>(
            pInsn->address + pInsn->size +    // RIP
            pInsn->detail->x86.operands[1].mem.disp
        );

        auto StringPtr = _LibccDllInterpreter.RvaToPointer<char>(StringRva);

        return 
            StringPtr &&
            strncmp(StringPtr, reinterpret_cast<const char*>(Keywords[i].Data), Keywords[i].Length) == 0 &&
            StringPtr[Keywords[i].Length] == '\x00';
    } else {
        return false;
    }
}
#else
bool PatchSolution3::CheckIfFound(cs_insn* pInsn, size_t i) const {
    auto& op_count = pInsn->detail->x86.op_count;
    auto& operands = pInsn->detail->x86.operands;

    if (op_count < 1 || operands[op_count - 1].type != X86_OP_IMM)
        return false;

    if (Keywords[i].Type == IMM_DATA) {
        static_assert(sizeof(operands[op_count - 1].imm) == sizeof(Keywords[i].Data));
        return
            operands[op_count - 1].imm == *reinterpret_cast<const int64_t*>(Keywords[i].Data) &&
            pInsn->detail->x86.encoding.imm_size == Keywords[i].Length;
    } else if (Keywords[i].Type == STRING_DATA) {
        auto StringRva = static_cast<uintptr_t>(
            operands[op_count - 1].imm - 
            _LibccDllInterpreter.GetImageNTHeaders()->OptionalHeader.ImageBase
        );

        auto StringPtr = _LibccDllInterpreter.RvaToPointer<char>(StringRva);

        return 
            StringPtr &&
            strncmp(StringPtr, reinterpret_cast<const char*>(Keywords[i].Data), Keywords[i].Length) == 0 &&
            StringPtr[Keywords[i].Length] == '\x00';
    } else {
        return false;
    }
}
#endif

#if defined(_M_AMD64)
PatchSolution3::PatchPointInfo 
PatchSolution3::CreatePatchPoint(const uint8_t* pOpcode, cs_insn* pInsn, size_t i) const {
    PatchPointInfo result;

    result.Opcode.ConstPtr = pOpcode;
    result.OpcodeRva = pInsn->address;

    if (pInsn->detail->x86.operands[1].type == X86_OP_MEM) {
        auto StringRva = static_cast<uintptr_t>(
            pInsn->address + pInsn->size +    // RIP
            pInsn->detail->x86.operands[1].mem.disp
        );

        auto StringPtr = _LibccDllInterpreter.RvaToPointer<char>(StringRva);

        result.OriginalStringPtr = StringPtr;

        if (Keywords[i].NotRecommendedToModify == false) {
            result.PatchPtr = reinterpret_cast<uint8_t*>(result.OriginalStringPtr);
            result.PatchSize = Keywords[i].Length;
        } else {
            result.PatchPtr = result.Opcode.Ptr + pInsn->detail->x86.encoding.disp_offset;
            result.PatchSize = pInsn->detail->x86.encoding.disp_size;
        }
    } else {                                            // X86_OP_IMM
        result.PatchPtr = result.Opcode.Ptr + pInsn->detail->x86.encoding.imm_offset;
        result.PatchSize = pInsn->detail->x86.encoding.imm_size;
        result.OriginalStringPtr = nullptr;
    }

    result.ReplaceStringPtr = nullptr;

    return result;
}
#else       // for i386
PatchSolution3::PatchPointInfo
PatchSolution3::CreatePatchPoint(const uint8_t* pOpcode, cs_insn* pInsn, size_t i) const {
    PatchPointInfo result;

    result.Opcode.ConstPtr = pOpcode;
    result.OpcodeRva = pInsn->address;

    if (Keywords[i].Type == IMM_DATA) {
        result.PatchPtr = result.Opcode.Ptr + pInsn->detail->x86.encoding.imm_offset;
        result.PatchSize = pInsn->detail->x86.encoding.imm_size;
        result.OriginalStringPtr = nullptr;
    } else {
        auto StringRva = static_cast<uintptr_t>(
            pInsn->detail->x86.operands[0].imm - 
            _LibccDllInterpreter.GetImageNTHeaders()->OptionalHeader.ImageBase
        );

        auto StringPtr = _LibccDllInterpreter.RvaToPointer<char>(StringRva);

        result.OriginalStringPtr = StringPtr;

        if (Keywords[i].NotRecommendedToModify == false) {
            result.PatchPtr = reinterpret_cast<uint8_t*>(result.OriginalStringPtr);
            result.PatchSize = Keywords[i].Length;
        } else {
            result.PatchPtr = result.Opcode.Ptr + pInsn->detail->x86.encoding.imm_offset;
            result.PatchSize = pInsn->detail->x86.encoding.imm_size;
        }
    }

    result.ReplaceStringPtr = nullptr;

    return result;
}
#endif

CapstoneDisassembler::Context
PatchSolution3::GetJumpedBranch(const CapstoneDisassembler::Context& NotJumpedBranch, cs_insn* pJxxInsn) const {
    CapstoneDisassembler::Context JumpedBranch;

    JumpedBranch.Opcode.Ptr = 
        _LibccDllInterpreter.RvaToPointer<uint8_t>(
            static_cast<uintptr_t>(pJxxInsn->detail->x86.operands[0].imm)
        );

    JumpedBranch.OpcodesSize = 
        NotJumpedBranch.OpcodesSize - (JumpedBranch.Opcode.Ptr - NotJumpedBranch.Opcode.Ptr);

    JumpedBranch.Address = pJxxInsn->detail->x86.operands[0].imm;

    return JumpedBranch.Opcode.Ptr ? JumpedBranch : CapstoneDisassembler::InvalidContext;
}

CapstoneDisassembler::Context
PatchSolution3::HandleJcc(const CapstoneDisassembler::Context& NotJumpedBranch,
                          const CapstoneDisassembler::Context& JumpedBranch,
                          size_t i) const {
    CapstoneDisassembler::Context A = NotJumpedBranch;
    CapstoneDisassembler::Context B = JumpedBranch;
    int WeightA = 0;
    int WeightB = 0;
    CapstoneDisassembler Disassembler = _CapstoneEngine.CreateDisassembler();

    while (true) {
        int WeightAPrev = WeightA;
        int WeightBPrev = WeightB;

        //
        // process NotJumpedBranch
        //
        Disassembler.SetContext(A);
        while (Disassembler.Next()) {
            //
            // For all x86 mnemonics, only 'jcc' or 'jmp' starts with 'j' or 'J'.
            // So it should be a new branch if we meet them.
            //
            if (Disassembler.Instruction()->mnemonic[0] == 'j' || Disassembler.Instruction()->mnemonic[0] == 'J') {
                auto JumpedBranch = GetJumpedBranch(Disassembler.GetContext(), Disassembler.Instruction());
                if (JumpedBranch == CapstoneDisassembler::InvalidContext)
                    return CapstoneDisassembler::InvalidContext;

                if (_stricmp(Disassembler.Instruction()->mnemonic, "jmp") == 0) {
                    Disassembler.SetContext(JumpedBranch);
                } else {
                    Disassembler.SetContext(HandleJcc(Disassembler.GetContext(), JumpedBranch, i));
                    if (Disassembler.GetContext() == CapstoneDisassembler::InvalidContext)
                        break;
                }
            } else if (_stricmp(Disassembler.Instruction()->mnemonic, "ret") == 0) {
                return JumpedBranch;
            } else {
                if (CheckIfMatchPattern(Disassembler.Instruction()) == false)
                    continue;

                //
                // if match pattern, but keyword doesn't match, 
                // NotJumpedBranch must not be what we want
                //
                if (CheckIfFound(Disassembler.Instruction(), i) == false)
                    return JumpedBranch;

                //
                // If keyword is succeeded to match
                // Add WeightA and stop processing NotJumpedBranch
                //
                WeightA++;
                break;
            }
        }
        A = Disassembler.GetContext();

        //
        // process JumpedBranch
        //
        Disassembler.SetContext(B);
        while (Disassembler.Next()) {
            //
            // For all x86 mnemonics, only 'jcc' or 'jmp' starts with 'j' or 'J'.
            // So it should be a new branch if we meet them.
            //
            if (Disassembler.Instruction()->mnemonic[0] == 'j' || Disassembler.Instruction()->mnemonic[0] == 'J') {
                auto JumpedBranch = GetJumpedBranch(Disassembler.GetContext(), Disassembler.Instruction());
                if (JumpedBranch == CapstoneDisassembler::InvalidContext)
                    return CapstoneDisassembler::InvalidContext;

                if (_stricmp(Disassembler.Instruction()->mnemonic, "jmp") == 0) {
                    Disassembler.SetContext(JumpedBranch);
                } else {
                    Disassembler.SetContext(HandleJcc(Disassembler.GetContext(), JumpedBranch, i));
                    if (Disassembler.GetContext() == CapstoneDisassembler::InvalidContext)
                        break;
                }
            } else if (_stricmp(Disassembler.Instruction()->mnemonic, "ret") == 0) {
                return NotJumpedBranch;
            } else {
                if (CheckIfMatchPattern(Disassembler.Instruction()) == false)
                    continue;

                if (CheckIfFound(Disassembler.Instruction(), i) == false)
                    return NotJumpedBranch;

                WeightB++;
                break;
            }
        }
        B = Disassembler.GetContext();

        if (WeightAPrev == WeightA && WeightBPrev == WeightB)
            return CapstoneDisassembler::InvalidContext;

        if (WeightA != WeightB)
            return WeightA > WeightB ? NotJumpedBranch : JumpedBranch;
        else
            i++;
    }
}

PatchSolution3::PatchSolution3() :
#if defined(_M_IX86)
    _CapstoneEngine(CS_ARCH_X86, CS_MODE_32),
#else
    _CapstoneEngine(CS_ARCH_X86, CS_MODE_64),
#endif
    _Patches{}
{
    _CapstoneEngine.Option(CS_OPT_DETAIL, CS_OPT_ON);
}

void PatchSolution3::SetFile(void* pFile) {
    if (!_LibccDllInterpreter.ParseImage(pFile, false)) {
        throw Exception(__BASE_FILE__, __LINE__,
                        TEXT("Invalid PE file."));
    }
}

bool PatchSolution3::FindPatchOffset() noexcept {
    PatchPointInfo Patches[111] = {};

    uint8_t* pView = _LibccDllInterpreter.GetImageBase<uint8_t>();
    PIMAGE_SECTION_HEADER ptextSectionHeader = _LibccDllInterpreter.GetSectionHeader(".text");
    uint8_t* ptextSection = pView + ptextSectionHeader->PointerToRawData;
    off_t TargetFunctionOffset = -1; // relative to ptextSection
    
    if (ptextSectionHeader == nullptr)
        return false;

    for (DWORD i = 0; i < ptextSectionHeader->SizeOfRawData; ++i) {
        const uint32_t Hint = 0x6b67424e;
        if (*reinterpret_cast<uint32_t*>(ptextSection + i) == Hint) {
#if defined(_M_AMD64)
            static const uint8_t HeadBytesOfTargetFunction[] = {
                0x40, 0x55,                                         // push    rbp
                0x48, 0x8D, 0xAC, 0x24, 0x70, 0xBC, 0xFF, 0xFF,     // lea     rbp, [rsp-4390h]
                0xB8, 0x90, 0x44, 0x00, 0x00                        // mov     eax, 4490h
            };
            for (DWORD j = i - 0x250; j < i; ++j) {
#else
            static const uint8_t HeadBytesOfTargetFunction[] = {
                0x55,           // push    ebp
                0x8B, 0xEC,     // mov     ebp, esp
                0x6A, 0xFF      // push    0xffffffff
            };
            for (DWORD j = i - 0x1B0; j < i; ++j) {
#endif
                if (memcmp(ptextSection + j, HeadBytesOfTargetFunction, sizeof(HeadBytesOfTargetFunction)) == 0) {
                    TargetFunctionOffset = j;
                    break;
                }
            }

            break;
        }
    }

    if (TargetFunctionOffset == -1)
        return false;

    size_t KeywordIndex = 0;

    {
        CapstoneDisassembler Disassembler = _CapstoneEngine.CreateDisassembler();
        Disassembler.SetContext(
            ptextSection + TargetFunctionOffset,
#if defined(_M_AMD64)
            0xcd03,
#else
            0x9014,
#endif
            ptextSectionHeader->VirtualAddress + TargetFunctionOffset
        );

        while (Disassembler.Next()) {
            if (Disassembler.Instruction()->mnemonic[0] == 'j' || Disassembler.Instruction()->mnemonic[0] == 'J') {
                auto JumpedBranch = GetJumpedBranch(Disassembler.GetContext(), Disassembler.Instruction());
                if (JumpedBranch == CapstoneDisassembler::InvalidContext)
                    return false;

                if (_stricmp(Disassembler.Instruction()->mnemonic, "jmp") == 0) {
                    Disassembler.SetContext(JumpedBranch);
                } else {
                    Disassembler.SetContext(HandleJcc(Disassembler.GetContext(), JumpedBranch, KeywordIndex));
                    if (Disassembler.GetContext() == CapstoneDisassembler::InvalidContext)
                        return false;
                }
            } else if (_stricmp(Disassembler.Instruction()->mnemonic, "ret") == 0) {
                return false;
            } else {
                if (CheckIfMatchPattern(Disassembler.Instruction()) == false)
                    continue;

                if (CheckIfFound(Disassembler.Instruction(), KeywordIndex) == false)
                    return false;
                
                Patches[KeywordIndex] =
                    CreatePatchPoint(Disassembler.InstructionContext().Opcode.Ptr,
                                     Disassembler.Instruction(), 
                                     KeywordIndex);

                KeywordIndex++;
            }

            if (KeywordIndex == 111)
                break;
        }
    }
    
    if (KeywordIndex != 111)
        return false;

    _tprintf_s(TEXT("[*] PatchSolution3 ...... Ready to apply\n"));
    for (size_t i = 0; i < 111; ++i) {
        _Patches[i] = Patches[i];
        _tprintf_s(TEXT(" |--[*] PatchPoint[%zu]\n"), i);
        _tprintf_s(TEXT(" |   | - Instruction Offset = +0x%.16zx\n"), _Patches[i].Opcode.Ptr - pView);
        _tprintf_s(TEXT(" |   | - Instruction RVA    = +0x%.16llx\n"), _Patches[i].OpcodeRva);
        _tprintf_s(TEXT(" |   | - Patch Offset       = +0x%.16zx\n"), _Patches[i].PatchPtr - pView);
        _tprintf_s(TEXT(" |\n"));
    }

    return true;
}

// Brute-force search, str_l should be 1 or 2
static off_t SearchString(const void* begin_p, size_t range_s, const char* str_p, size_t str_l) {
    const char* char_ptr = reinterpret_cast<const char*>(begin_p);
    for (size_t i = 0; i < range_s; ++i) {
        if (char_ptr[i] == str_p[0]) {
            bool match = true;

            for (size_t j = 1; j < str_l; ++j) {
                if (char_ptr[i + j] != str_p[j]) {
                    match = false;
                    break;
                }
            }

            if (match && char_ptr[i + str_l] == '\x00')
                return static_cast<off_t>(i);
        }
    }
    return -1;
}

bool PatchSolution3::CheckKey(RSACipher* pCipher) const {
    std::string PublicKeyString = 
        pCipher->ExportKeyString<RSAKeyType::PublicKey, RSAKeyFormat::PEM>();

    Helper::StringRemove<std::string>(PublicKeyString, "-----BEGIN PUBLIC KEY-----");
    Helper::StringRemove<std::string>(PublicKeyString, "-----END PUBLIC KEY-----");
    Helper::StringRemove<std::string>(PublicKeyString, "\n");

    if(PublicKeyString.length() != 0x188)
        return false;
    
    char* pView = _LibccDllInterpreter.GetImageBase<char>();
    PIMAGE_SECTION_HEADER prdataSectionHeader = _LibccDllInterpreter.GetSectionHeader(".rdata");
    char* prdataSection = pView + prdataSectionHeader->PointerToRawData;

    size_t readptr = 0;
    for (size_t i = 0; i < 111; ++i) {
        if (Keywords[i].NotRecommendedToModify) {
            off_t offset = 0;

            while (true) {
                off_t off = SearchString(_Patches[i].OriginalStringPtr + offset,
                                         prdataSectionHeader->SizeOfRawData - (_Patches[i].OriginalStringPtr - prdataSection) - offset,
                                         PublicKeyString.c_str() + readptr,
                                         Keywords[i].Length);

                if (off == -1)
                    return false;
                else
                    offset += off;

                uintptr_t ReplaceStringRva =
                    prdataSectionHeader->VirtualAddress + (_Patches[i].OriginalStringPtr - prdataSection) +   // OriginalString Rva
                    offset;

                if (_LibccDllInterpreter.IsRvaRangeInRelocTable(ReplaceStringRva, Keywords[i].Length + 1) == false)
                    //
                    // ReplaceString won't be modified due to relocation
                    //   which can be used to act as a part of public key string
                    //
                    break;  
                else
                    //
                    // Damn it!
                    // ReplaceString will be modified during relocation
                    // We have to find another one
                    //
                    ++offset;
            }

            _Patches[i].ReplaceStringPtr = _Patches[i].OriginalStringPtr + offset;
        }
        readptr += Keywords[i].Length;
    }

    return true;
}

void PatchSolution3::MakePatch(RSACipher* pCipher) const {
    std::string PublicKeyString = 
        pCipher->ExportKeyString<RSAKeyType::PublicKey, RSAKeyFormat::PEM>();

    Helper::StringRemove<std::string>(PublicKeyString, "-----BEGIN PUBLIC KEY-----");
    Helper::StringRemove<std::string>(PublicKeyString, "-----END PUBLIC KEY-----");
    Helper::StringRemove<std::string>(PublicKeyString, "\n");

    uint8_t* pView = _LibccDllInterpreter.GetImageBase<uint8_t>();

    _putts(TEXT("******************************************"));
    _putts(TEXT("*            PatchSulution3              *"));
    _putts(TEXT("******************************************"));

    size_t readptr = 0;
    for (size_t i = 0; i < _countof(_Patches); ++i) {
        _tprintf_s(TEXT("@ +%08zx: "), _Patches[i].PatchPtr - pView);
        Helper::PrintBytes(_Patches[i].PatchPtr, _Patches[i].PatchSize);

        _tprintf_s(TEXT(" ---> "));
        if (Keywords[i].NotRecommendedToModify == false) {
            memcpy(_Patches[i].PatchPtr, PublicKeyString.c_str() + readptr, Keywords[i].Length);
        } else {
            auto offset = _Patches[i].ReplaceStringPtr - _Patches[i].OriginalStringPtr;

            union {
                uint8_t bytes[8];
                uint64_t qword;
            } disp = {};

            memcpy(disp.bytes, _Patches[i].PatchPtr, _Patches[i].PatchSize);
            disp.qword += offset;
            memcpy(_Patches[i].PatchPtr, disp.bytes, _Patches[i].PatchSize);
        }
        readptr += Keywords[i].Length;

        Helper::PrintBytes(_Patches[i].PatchPtr, _Patches[i].PatchSize);
        _tprintf_s(TEXT("\n"));
    }
    return;
}



