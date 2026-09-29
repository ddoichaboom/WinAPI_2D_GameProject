#pragma once
#include "CUi.h"
class CScore :
    public CUi
{
public:
    CScore();
    virtual ~CScore();

public:
    void Initialize() override;
    int Update(float deltaTime) override;
    void Late_Update(float deltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    void    Render_Number(HDC hDC, long number, float fStartX, float fStartY);
    float   Get_Formatted_Number_Width(long number);

private:
    const TCHAR*        m_pScoreIconKey;
    const TCHAR*        m_pCoinIconKey;
    const TCHAR*        m_pNumberFontKey;
    const TCHAR*        m_pCommaFontKey;


    const IMAGEDATA*    m_pScoreIconData;
    const IMAGEDATA*    m_pCoinIconData;
    const IMAGEDATA*    m_pNumberFontInfo;
    const IMAGEDATA*    m_pCommaFontInfo;


    long                m_lScore;
    long                m_lCoin;

};

