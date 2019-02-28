#include "CapstoneDisassembler.hpp"
#include <memory.h>

// libs required by capstone
#pragma comment(lib, "legacy_stdio_definitions.lib")
#if defined(_M_AMD64)
#pragma comment(lib, "capstone_static.lib")
#else
#pragma comment(lib, "capstone.lib")
#endif

// ----------- avoid link error caused by capstone_static.lib
#define stdin  (__acrt_iob_func(0))
#define stdout (__acrt_iob_func(1))
#define stderr (__acrt_iob_func(2))

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE* __cdecl __iob_func(void) {
    return _iob; 
}
// ------------

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("CapstoneDisassembler.cpp")

bool CapstoneDisassembler::Context::operator==(const CapstoneDisassembler::Context& Other) const noexcept {
    return memcmp(this, &Other, sizeof(Context)) == 0;
}

bool CapstoneDisassembler::Context::operator!=(const CapstoneDisassembler::Context& Other) const noexcept {
    return memcmp(this, &Other, sizeof(Context)) != 0;
}

CapstoneDisassembler::CapstoneDisassembler(const CapstoneEngine& Engine) :
    _EngineRef(Engine),
    _Context(InvalidContext),
    _InsnPtr(nullptr) 
{
    cs_insn* insn;
    insn = cs_malloc(_EngineRef);
    if (insn == nullptr)
        throw CapstoneException(__BASE_FILE__, __LINE__, cs_errno(_EngineRef),
                                TEXT("cs_malloc failed."));
    else
        _InsnObj.TakeOver(insn);
}

void CapstoneDisassembler::SetContext(const uint8_t* Opcodes, size_t Size, uint64_t Address) noexcept {
    _Context.Opcode.ConstPtr = Opcodes;
    _Context.OpcodesSize = Size;
    _Context.Address = Address;
    _InsnPtr = nullptr;
}

void CapstoneDisassembler::SetContext(const CapstoneDisassembler::Context& Context) noexcept {
    _Context = Context;
    _InsnPtr = nullptr;
}

const CapstoneDisassembler::Context& CapstoneDisassembler::GetContext() const noexcept {
    return _Context;
}

bool CapstoneDisassembler::Next() noexcept {
    bool bOk = cs_disasm_iter(_EngineRef,
                              &_Context.Opcode.ConstPtr,
                              &_Context.OpcodesSize,
                              &_Context.Address,
                              _InsnObj);
    if (bOk) {
        if (_InsnPtr == nullptr)
            _InsnPtr = _InsnObj;
    } else {
        _InsnPtr = nullptr;
    }
    return bOk;
}

cs_insn* CapstoneDisassembler::Instruction() const noexcept {
    return _InsnPtr;
}

CapstoneDisassembler::Context CapstoneDisassembler::InstructionContext() const noexcept {
    Context ContextOfInsn;
    ContextOfInsn.Opcode.Ptr = _Context.Opcode.Ptr - _InsnPtr->size;
    ContextOfInsn.OpcodesSize = _Context.OpcodesSize + _InsnPtr->size;
    ContextOfInsn.Address = _Context.Address - _InsnPtr->size;
    return ContextOfInsn;
}

CapstoneEngine::CapstoneEngine(cs_arch ArchType, cs_mode Mode) {
    cs_err status;
    csh handle;

    status = cs_open(ArchType, Mode, &handle);
    if (status != CS_ERR_OK)
        throw CapstoneException(__BASE_FILE__, __LINE__, status,
                                TEXT("cs_open failed."));
    else
        _EngineObj.TakeOver(handle);
}

CapstoneEngine::operator csh() const noexcept {
    return _EngineObj.Get();
}

void CapstoneEngine::Option(cs_opt_type Type, size_t Value) {
    cs_err status;
    status = cs_option(_EngineObj, Type, Value);
    if (status != CS_ERR_OK)
        throw CapstoneException(__BASE_FILE__, __LINE__, status,
                                TEXT("cs_open failed."));
}

CapstoneDisassembler CapstoneEngine::CreateDisassembler() const {
    return CapstoneDisassembler(*this);
}

