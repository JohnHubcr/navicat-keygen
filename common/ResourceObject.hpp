#pragma once
#include <type_traits>

template<typename __ResourceTraits>
class ResourceObject {
public:
    using HandleType = typename __ResourceTraits::HandleType;
private:
    HandleType _Handle;
public:

    ResourceObject() noexcept :
        _Handle(__ResourceTraits::InvalidValue) {}

    explicit ResourceObject(const HandleType& Handle) noexcept :
        _Handle(Handle) {}

    //
    // Copy constructor is not allowed
    //
    ResourceObject(const ResourceObject<__ResourceTraits>& Other) = delete;

    ResourceObject(ResourceObject<__ResourceTraits>&& Other) noexcept : 
        _Handle(Other._Handle)
    { 
        Other._Handle = __ResourceTraits::InvalidValue; 
    }

    //
    // Copy assignment is not allowed
    //
    ResourceObject<__ResourceTraits>&
    operator=(const ResourceObject<__ResourceTraits>& Other) = delete;

    ResourceObject<__ResourceTraits>&
    operator=(ResourceObject<__ResourceTraits>&& Other) noexcept {
        _Handle = Other._Handle;
        Other._Handle = __ResourceTraits::InvalidValue;
        return *this;
    }

    template<typename __DummyType = int,
             typename = typename std::enable_if<std::is_pointer<HandleType>::value, __DummyType>::type>
    HandleType operator->() const noexcept {
        return _Handle;
    }

    operator HandleType() const noexcept {
        return _Handle;
    }

    // Check if handle is a valid handle
    bool IsValid() const noexcept {
        return _Handle != __ResourceTraits::InvalidValue;
    }

    HandleType Get() const noexcept {
        return _Handle;
    }

    void TakeOver(const HandleType& Handle) noexcept {
        if (_Handle != __ResourceTraits::InvalidValue) {
            __ResourceTraits::Releasor(_Handle);
            _Handle = __ResourceTraits::InvalidValue;
        } else {
            _Handle = Handle;
        }
    }

    void Abandon() noexcept {
        _Handle = __ResourceTraits::InvalidValue;
    }

    // Force release
    void Release() {
        if (_Handle != __ResourceTraits::InvalidValue) {
            __ResourceTraits::Releasor(_Handle);
            _Handle = __ResourceTraits::InvalidValue;
        }
    }

    ~ResourceObject() {
        if (_Handle != __ResourceTraits::InvalidValue) {
            __ResourceTraits::Releasor(_Handle);
            _Handle = __ResourceTraits::InvalidValue;
        }
    }
};

template<typename __ClassType>
struct CppObjectTraits {
    using HandleType = __ClassType*;
    static inline const HandleType InvalidValue = nullptr;
    static inline void Releasor(HandleType pObject) {
        delete pObject;
    }
};

template<typename __ClassType>
struct CppDynamicArrayTraits {
    using HandleType = __ClassType*;
    static inline const HandleType InvalidValue = nullptr;
    static inline void Releasor(HandleType pArray) {
        delete[] pArray;
    }
};


