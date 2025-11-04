#pragma once
#include "CUi.h"
class CBonusTime :
    public CUi
{
public:
    CBonusTime();
    virtual ~CBonusTime();

public:

    void Initialize() override;
    int Update(float deltaTime) override;
    void Late_Update(float deltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    const TCHAR*        m_pBgFrameKey;
    const TCHAR*        m_pEffectFrameKey;

    const IMAGEDATA*    m_pBgInfo;
    const IMAGEDATA*    m_pEffectInfo;

    float               m_fEffectPosX;
    float               m_fEffectSpeed;

};

