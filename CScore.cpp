#include "pch.h"
#include "CScore.h"
#include "CDataMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"

CScore::CScore()

   : m_pScoreIconKey(L"UI_ICON_SCORE"),
    m_pCoinIconKey(L"UI_ICON_COIN"),
    m_pNumberFontKey(L"UI_FONT_NUMBER"),
    m_pCommaFontKey(L"UI_FONT_COMMA"),
    m_pScoreIconData(nullptr),
    m_pCoinIconData(nullptr),
    m_pNumberFontInfo(nullptr),
    m_pCommaFontInfo(nullptr),
    m_lScore(0), m_lCoin(0)
{
}

CScore::~CScore()
{
    Release();
}

void CScore::Initialize()
{
    
    m_pScoreIconData = CDataMgr::Get_Instance()->Get_ImageData(m_pScoreIconKey);
    m_pCoinIconData = CDataMgr::Get_Instance()->Get_ImageData(m_pCoinIconKey);

    m_pNumberFontInfo = CDataMgr::Get_Instance()->Get_ImageData(m_pNumberFontKey);
    m_pCommaFontInfo = CDataMgr::Get_Instance()->Get_ImageData(m_pCommaFontKey);

}

int CScore::Update(float deltaTime)
{
    CObj* pPlayer = CObjMgr::Get_Instance()->Get_Target(PLAYER);
    if (pPlayer)
    {
        const PLAYERINFO* pPlayerInfo = static_cast<CPlayer*>(pPlayer)->Get_PlayerInfo();
        m_lScore = pPlayerInfo->lScore;
        m_lCoin = pPlayerInfo->lCoin;
    }

    return 0;
}

void CScore::Late_Update(float deltaTime)
{
}

void CScore::Render(HDC hDC)
{
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    // 1. 점수 렌더링 ( 중앙 정렬 )
    if (m_pScoreIconData && m_pNumberFontInfo && m_pCommaFontInfo)
    {
        float fScoreNumWidth = Get_Formatted_Number_Width(m_lScore);
        float fIconWidth = m_pScoreIconData->tInfo.fCX;
        float fSpace = 10.f;                                // 아이콘과 숫자 텍스트 사이 여유공간
        float fTotalWidth = fIconWidth + fSpace + fScoreNumWidth;
        float fStartX = (WINCX / 2.f) - (fTotalWidth / 2.f);
        float fScoreY = 100.f;

        HDC hScoreIconDC = CBmpMgr::Get_Instance()->Find_Image(m_pScoreIconKey);
        if (hScoreIconDC)
        {
            AlphaBlend(hDC,
                (int)fStartX, (int)(fScoreY - (m_pScoreIconData->tInfo.fCY / 2.f)),
                (int)fIconWidth, (int)m_pScoreIconData->tInfo.fCY,
                hScoreIconDC,
                0, 0,
                (int)fIconWidth, (int)m_pScoreIconData->tInfo.fCY,
                bf);
        }
        Render_Number(hDC, m_lScore, fStartX + fIconWidth + fSpace, fScoreY);

    }
    // 2. 코인 렌더링 ( 좌측 정렬 )
    if (m_pCoinIconData && m_pNumberFontInfo && m_pCommaFontInfo)
    {
        float fCoinIconX = 50.f;
        float fCoinIconY = 150.f;
        float fSpace = 10.f;

        HDC hCoinIconDC = CBmpMgr::Get_Instance()->Find_Image(m_pCoinIconKey);
        if (hCoinIconDC)
        {
            AlphaBlend(hDC,
                (int)fCoinIconX,
                (int)(fCoinIconY - (m_pCoinIconData->tInfo.fCY / 2.f)),
                (int)m_pCoinIconData->tInfo.fCX, (int)m_pCoinIconData->tInfo.fCY,
                hCoinIconDC,
                0, 0,
                (int)m_pCoinIconData->tInfo.fCX, (int)m_pCoinIconData->tInfo.fCY,
                bf);
        }
        Render_Number(hDC, m_lCoin, fCoinIconX + m_pCoinIconData->tInfo.fCX + fSpace, fCoinIconY);

    }
}

void CScore::Release()
{

}

float CScore::Get_Formatted_Number_Width(long number)
{
    string strNum = to_string(number);
    if (strNum.empty())
        return 0.f;

    int iCommaCount = (strNum.length() - 1) / 3;

    float fNumWidth = m_pNumberFontInfo->tInfo.fCX / 10.f;
    float fCommaWidth = m_pCommaFontInfo->tInfo.fCX;

    return (strNum.length() * fNumWidth) + (iCommaCount * fCommaWidth);

}

void CScore::Render_Number(HDC hDC, long number, float fStartX, float fStartY)
{
    HDC hNumberDC = CBmpMgr::Get_Instance()->Find_Image(m_pNumberFontKey);
    HDC hCommaDC = CBmpMgr::Get_Instance()->Find_Image(m_pCommaFontKey);

    if (!hNumberDC || !hCommaDC)
        return;

    string strNum = to_string(number);
    string formattedNum;
    int digitCount = 0;
    for (int i = strNum.length() - 1; i >= 0; --i)
    {
        formattedNum.insert(0, 1, strNum[i]);
        digitCount++;
        if (((digitCount % 3) == 0) && (i > 0))
        {
            formattedNum.insert(0, 1, ',');
        }
    }

    float fNumWidth     = m_pNumberFontInfo->tInfo.fCX / 10.f;
    float fNumHeight    = m_pNumberFontInfo->tInfo.fCY;
    float fCommaWidth   = m_pCommaFontInfo->tInfo.fCX;
    float fCommaHeight  = m_pCommaFontInfo->tInfo.fCY;
    float fCurrentX     = fStartX;

    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    for (char c : formattedNum)
    {
        if (c == ',')
        {
            AlphaBlend(hDC,
                (int)fCurrentX, (int)(fStartY - fCommaHeight / 2.f),
                (int)fCommaWidth, (int)fCommaHeight,
                hCommaDC,
                0, 0,
                (int)fCommaWidth, (int)fCommaHeight,
                bf);
            fCurrentX += fCommaWidth;
        }
        else
        {
            int iNum = c - '0';     // 문자 0을 숫자 0으로 , 문자 '1'을 숫자 1로 변환
            AlphaBlend(hDC,
                (int)fCurrentX, (int)(fStartY - fNumHeight / 2.f),
                (int)fNumWidth, (int)fNumHeight,
                hNumberDC,
                (int)(iNum * fNumWidth), 0,
                (int)fNumWidth, (int)fNumHeight,
                bf);
            fCurrentX += fNumWidth;

        }
    }

}


