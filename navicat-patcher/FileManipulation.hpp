#pragma once
#include <windows.h>
#include "ExceptionSystem.hpp"
#include "ResourceObject.hpp"

#undef __BASE_FILE__
#define __BASE_FILE__ TEXT("FileManipulation.hpp")

struct GenericHandleTraits {
    using HandleType = HANDLE;
    static inline const HandleType InvalidValue = NULL;
    static constexpr auto& Releasor = CloseHandle;
};

struct FileHandleTraits {
    using HandleType = HANDLE;
    static inline const HandleType InvalidValue = INVALID_HANDLE_VALUE;
    static constexpr auto& Releasor = CloseHandle;
};

struct MapViewTraits {
    using HandleType = PVOID;
    static inline const HandleType InvalidValue = NULL;
    static constexpr auto& Releasor = UnmapViewOfFile;
};

class FileObject {
private:
    ResourceObject<FileHandleTraits> _Obj;

    explicit FileObject(HANDLE Handle) noexcept:
        _Obj(Handle) {}
public:

    static bool IsExist(const TString& FileName) {
        DWORD dwAttributes = GetFileAttributes(FileName.c_str());
        if (dwAttributes == INVALID_FILE_ATTRIBUTES) {
            if (GetLastError() == ERROR_FILE_NOT_FOUND)
                return false;
            else
                throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                      TEXT("GetFileAttributes failed."));
        } else {
            return (dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
        }
    }

    template<typename __ReturnType>
    static __ReturnType Create(const TString& FileName, DWORD DesireAccess, DWORD ShareMode) {
        static_assert(std::is_same<__ReturnType, FileObject>::value || 
                      std::is_same<__ReturnType, FileObject*>::value);
        HANDLE Handle = ::CreateFile(FileName.c_str(),
                                     DesireAccess,
                                     ShareMode,
                                     NULL,
                                     CREATE_ALWAYS,
                                     FILE_ATTRIBUTE_NORMAL,
                                     NULL);
        if (Handle == INVALID_HANDLE_VALUE) {
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("CreateFile failed."));
        }

        if constexpr (std::is_same<__ReturnType, FileObject>::value)
            return FileObject(Handle);
        else
            return new FileObject(Handle);
    }

    template<typename __ReturnType>
    static __ReturnType Open(const TString& FileName, DWORD DesireAccess, DWORD ShareMode) {
        static_assert(std::is_same<__ReturnType, FileObject>::value || 
                      std::is_same<__ReturnType, FileObject*>::value);
        HANDLE FileHandle = ::CreateFile(FileName.c_str(),
                                         DesireAccess,
                                         ShareMode,
                                         NULL,
                                         OPEN_EXISTING,
                                         FILE_ATTRIBUTE_NORMAL,
                                         NULL);
        if (FileHandle == INVALID_HANDLE_VALUE) {
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("CreateFile failed."));
        }

        if constexpr (std::is_same<__ReturnType, FileObject>::value)
            return FileObject(FileHandle);
        else
            return new FileObject(FileHandle);
    }

    HANDLE GetHandle() const noexcept {
        return _Obj.Get();
    }

    UINT64 GetSize() const {
        LARGE_INTEGER Val;
        if (GetFileSizeEx(_Obj, &Val)) {
            return Val.QuadPart;
        } else {
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("GetFileSizeEx failed."));
        }
    }

    void Destroy() {
        _Obj.Release();
    }
};

class FileMapViewObject {
private:
    ResourceObject<GenericHandleTraits> _MapObj;
    ResourceObject<MapViewTraits> _ViewObj;

    explicit FileMapViewObject(HANDLE MapHandle) noexcept :
        _MapObj(MapHandle) {}
public:

    template<typename __ReturnType>
    static __ReturnType Create(HANDLE FileHandle, DWORD ProtectAttributes) {
        static_assert(std::is_same<__ReturnType, FileMapViewObject>::value ||
                      std::is_same<__ReturnType, FileMapViewObject*>::value);
        HANDLE MapHandle = CreateFileMapping(FileHandle, 
                                             NULL, 
                                             ProtectAttributes, 
                                             0, 
                                             0, 
                                             NULL);
        if (MapHandle == NULL) {
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("CreateFileMapping failed."));
        }

        if constexpr (std::is_same<__ReturnType, FileMapViewObject>::value)
            return FileMapViewObject(MapHandle);
        else
            return new FileMapViewObject(MapHandle);
    }

    void Map(DWORD DesiredAccess, UINT64 Offset = 0, SIZE_T Size = 0) {
        PVOID pView = MapViewOfFile(_MapObj,
                                    DesiredAccess,
                                    static_cast<DWORD>(Offset >> 32),
                                    static_cast<DWORD>(Offset),
                                    Size);
        if (pView == NULL) {
            throw SystemException(__BASE_FILE__, __LINE__, GetLastError(),
                                  TEXT("MapViewOfFile failed."));
        } else {
            _ViewObj.TakeOver(pView);
        }
    }

    void Unmap() {
        _ViewObj.Release();
    }

    void Destory() {
        Unmap();
        _MapObj.Release();
    }

    template<typename __Type>
    __Type* View() const noexcept {
        return reinterpret_cast<__Type*>(_ViewObj.Get());
    }

    template<typename __Type>
    const __Type* ConstView() const noexcept {
        return reinterpret_cast<__Type*>(_ViewObj.Get());
    }
};

