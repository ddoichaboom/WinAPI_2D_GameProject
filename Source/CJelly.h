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
    void        Set_Frame();
    wchar_t     Get_Alphabet_Char() const { return m_cAlphabetChar; }

    
private:
    JELLYINFO   m_tJellyInfo;
    wstring     m_PrevFrameKey;
    wchar_t     m_cAlphabetChar;

};

