#pragma once
#include "CScene.h"
class CEdit :
    public CScene
{
public:
    CEdit();
    virtual ~CEdit();

public:

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


private:
    void            Key_Input();
    void            Edit_State(HDC hDC);

private:
    POINT			m_tCursor;
    bool            m_bTestMode;
    bool            m_bEditMode;
    bool            m_bRenderVerticalLine;

    int             m_iSequence;        // 1Àº 1Ãþ ÇÃ·§Æû, 2´Â 2Ãþ ÇÃ·§Æû, 3Àº Àå¾Ö¹° 4´Â Á©¸® ¹èÄ¡ ´Ü°è 5´Â ´Ü°è ³¡

};

