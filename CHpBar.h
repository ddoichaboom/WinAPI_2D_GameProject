#pragma once
#include "CUi.h"
class CHpBar :
    public CUi
{
public:
    CHpBar();
    virtual ~CHpBar();

public:
    void    Initialize() override;
    int     Update(float fDeltaTime) override;
    void    Late_Update(float fDeltaTime) override;
    void    Render(HDC hDC) override;
    void    Release() override;

private:
    const TCHAR* m_pBgFrameKey;
    const TCHAR* m_pGaugeFrameKey;

    const IMAGEDATA*     m_pBgInfo;
    const IMAGEDATA*     m_pGaugeInfo;

    float            m_fRatio;
    
};

