#include "pch.h"
#include "CMyBmp.h"

using namespace Gdiplus;

CMyBmp::CMyBmp() {}
CMyBmp::~CMyBmp() { Release(); }

// 확장자 소문자 비교 유틸
static bool EndsWith(const std::wstring& s, const std::wstring& suffix) {
    if (s.size() < suffix.size()) return false;
    auto a = s, b = suffix;
    std::transform(a.begin(), a.end(), a.begin(), ::towlower);
    std::transform(b.begin(), b.end(), b.begin(), ::towlower);
    return std::equal(b.rbegin(), b.rend(), a.rbegin());
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
    Release(); // 재로딩 대비

    // 1) 메모리 DC 준비
    HDC hScreen = GetDC(g_hWnd);
    m_hMemDC = CreateCompatibleDC(hScreen);
    ReleaseDC(g_hWnd, hScreen);

    // 파일 확장자로 PNG 여부 판단
    std::wstring path(pFilePath ? pFilePath : L"");
    bool isPng = EndsWith(path, L".png");

    if (!isPng) {
        // ===== BMP 경로 (기존) =====
        m_hBitmap = (HBITMAP)LoadImage(
            NULL,
            pFilePath,
            IMAGE_BITMAP,
            0, 0,
            LR_LOADFROMFILE | LR_CREATEDIBSECTION
        );
        if (!m_hBitmap) {
            // 로드 실패 시 안전장치
            m_hOldBmp = nullptr;
            m_width = m_height = 0;
            return;
        }

        // BMP의 크기 구하기
        BITMAP bm = {};
        GetObject(m_hBitmap, sizeof(BITMAP), &bm);
        m_width = bm.bmWidth;
        m_height = bm.bmHeight;

        m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
        return;
    }

    // ===== PNG 경로 (GDI+) =====
    // GDI+ Bitmap 로드
    Bitmap* png = Bitmap::FromFile(pFilePath, FALSE);
    if (!png || png->GetLastStatus() != Ok) {
        delete png;
        m_hBitmap = nullptr;
        m_hOldBmp = nullptr;
        m_width = m_height = 0;
        return;
    }

    m_width = (int)png->GetWidth();
    m_height = (int)png->GetHeight();

    // 32bpp 프리멀티플라이드 ARGB DIBSection 생성
    BITMAPINFO bi = {};
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = m_width;
    bi.bmiHeader.biHeight = -m_height; // top-down DIB
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;

    void* pBits = nullptr;
    m_hBitmap = CreateDIBSection(m_hMemDC, &bi, DIB_RGB_COLORS, &pBits, NULL, 0);
    if (!m_hBitmap || !pBits) {
        delete png;
        m_hBitmap = nullptr;
        m_hOldBmp = nullptr;
        m_width = m_height = 0;
        return;
    }

    m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

    // GDI+로 메모리 DC에 그리면 자동으로 프리멀티 ARGB가 된다
    {
        Graphics g(m_hMemDC);
        g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        g.SetPixelOffsetMode(PixelOffsetModeHighQuality);
        g.DrawImage(png, 0, 0, m_width, m_height);
    }

    delete png;
}

void CMyBmp::Release()
{
    if (m_hMemDC) {
        if (m_hOldBmp) SelectObject(m_hMemDC, m_hOldBmp);
        m_hOldBmp = nullptr;
    }
    if (m_hBitmap) {
        DeleteObject(m_hBitmap);
        m_hBitmap = nullptr;
    }
    if (m_hMemDC) {
        DeleteDC(m_hMemDC);
        m_hMemDC = nullptr;
    }
    m_width = m_height = 0;
}
