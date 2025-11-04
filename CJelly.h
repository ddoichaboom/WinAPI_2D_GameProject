#pragma once
#include "CObj.h"
class CJelly :
    public CObj
{
public:
    CJelly();
    virtual ~CJelly();

public:
    void                Initialize() override;
    int                 Update(float deltaTime) override;
    void                Late_Update(float deltaTime) override;
    void                Render(HDC hDC) override;
    void                Release() override;

public:
    JELLYINFO*  Get_JellyInfo() { return &m_tJellyInfo; }

    
private:
    JELLYINFO   m_tJellyInfo;


};

