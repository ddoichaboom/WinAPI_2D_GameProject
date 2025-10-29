#include "pch.h"
#include "TcharIO.h"

namespace Utils {

    bool WriteTString(HANDLE hFile, const TCHAR* s) {
        if (!s) s = TEXT("");

#if defined(UNICODE) || defined(_UNICODE)
        using tstring = std::wstring;
#else
        using tstring = std::string;
#endif

        tstring tmp(s);
        uint32_t len = static_cast<uint32_t>(tmp.size()); // 널 미포함
        DWORD dw = 0;

        if (!::WriteFile(hFile, &len, sizeof(len), &dw, nullptr) || dw != sizeof(len))
            return false;

        if (len == 0) return true;

        const void* buf = static_cast<const void*>(tmp.data());
        const DWORD bytes = len * sizeof(TCHAR);

        return ::WriteFile(hFile, buf, bytes, &dw, nullptr) && dw == bytes;
    }

    bool ReadTString(HANDLE hFile, std::basic_string<TCHAR>& out) {
        out.clear();

        DWORD dw = 0;
        uint32_t len = 0;

        if (!::ReadFile(hFile, &len, sizeof(len), &dw, nullptr))
            return false;          // 오류
        if (dw == 0) return false; // EOF

        if (dw != sizeof(len)) return false;

        if (len == 0) {
            out.clear();
            return true;
        }

        std::vector<TCHAR> buf(len);
        const DWORD bytes = len * sizeof(TCHAR);

        if (!::ReadFile(hFile, buf.data(), bytes, &dw, nullptr) || dw != bytes)
            return false;

        out.assign(buf.data(), buf.data() + len);
        return true;
    }

} // namespace Utils