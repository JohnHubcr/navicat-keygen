#pragma once
#include <capstone/capstone.h>
#include "ExceptionCapstone.hpp"
#include "ResourceObject.hpp"

struct CapstoneHandleTraits {
    using HandleType = csh;
    static inline const HandleType InvalidValue = 0;
    static inline void Releasor(HandleType Handle) {
        cs_close(&Handle);
    }
};

template<typename __Type>
struct CapstoneMallocTraits {
    using HandleType = __Type*;
    static inline const HandleType InvalidValue = nullptr;
    static inline void Releasor(HandleType Handle) {
        cs_free(Handle, 1);
    }
};

class CapstoneDisassembler {
    friend class CapstoneEngine;
public:

    struct Context {
        union {
            const uint8_t* ConstPtr;
            uint8_t* Ptr;
        } Opcode;
        size_t OpcodesSize;
        uint64_t Address;

        bool operator==(const Context& Other) const noexcept;
        bool operator!=(const Context& Other) const noexcept;
    };

    static inline Context InvalidContext = {};

private:
    const CapstoneEngine& _EngineRef;
    Context _Context;
    ResourceObject<CapstoneMallocTraits<cs_insn>> _InsnObj;
    cs_insn* _InsnPtr;

    explicit CapstoneDisassembler(const CapstoneEngine& Engine);
public:

    void SetContext(const uint8_t* Opcodes, size_t Size, uint64_t Address = 0) noexcept;
    void SetContext(const Context& Context) noexcept;

    const Context& GetContext() const noexcept;

    bool Next() noexcept;

    cs_insn* Instruction() const noexcept;

    Context InstructionContext() const noexcept;
};

class CapstoneEngine {
private:
    ResourceObject<CapstoneHandleTraits> _EngineObj;
public:

    CapstoneEngine(cs_arch ArchType, cs_mode Mode);

    operator csh() const noexcept;

    void Option(cs_opt_type Type, size_t Value);

    CapstoneDisassembler CreateDisassembler() const;
};

