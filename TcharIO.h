#pragma once
#include "pch.h"
#include <type_traits>


namespace Utils {

    // 길이-프리픽스(TCHAR 단위)로 문자열 저장/로드
    bool WriteTString(HANDLE hFile, const TCHAR* s);
    bool ReadTString(HANDLE hFile, std::basic_string<TCHAR>& out);

    // Trivially-copyable 타입(POD) 전용 바이너리 I/O
    template<typename T>
    inline bool WritePOD(HANDLE hFile, const T& v) {
        static_assert(std::is_trivially_copyable_v<T>,
            "WritePOD requires trivially copyable type");
        DWORD dw = 0;
        return ::WriteFile(hFile, &v, sizeof(T), &dw, nullptr) && dw == sizeof(T);
    }

    template<typename T>
    inline bool ReadPOD(HANDLE hFile, T& v) {
        static_assert(std::is_trivially_copyable_v<T>,
            "ReadPOD requires trivially copyable type");
        DWORD dw = 0;
        if (!::ReadFile(hFile, &v, sizeof(T), &dw, nullptr)) return false; // 오류
        return dw == sizeof(T); // EOF일 땐 false (dw==0)로 돌아옴
    }

} // namespace Utils



