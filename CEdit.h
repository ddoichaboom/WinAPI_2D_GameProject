#pragma once
#include "CScene.h"
class CEdit :
    public CScene
{
public:
    CEdit();
    virtual ~CEdit();

public:
    enum EDIT_TYPE { EDIT_FIRST_PLATFORM, EDIT_SECOND_PLATFORM, EDIT_OBSTACLE, EDIT_JELLY, EDIT_ITEM, EDIT_TYPE_END};

public:
    void Initialize() override;
    void Update(float fDeltaTime) override;
    void Late_Update(float fDeltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;


private:
    void            Key_Input();
    void            Edit_State(HDC hDC);
    void            Save_Chunk_Data(const TCHAR* pFilePath);

private:
    const TCHAR*    m_pFrameKey;
    const TCHAR*    m_pStageFrameKey;
    const TCHAR*    m_pTileFrameKey;        // Ã¹¹øÂ° ÇÃ·§Æû Àü¿ë ÇÁ·¹ÀÓ Å°



    POINT			m_tCursor;
    bool            m_bTestMode;
    bool            m_bEditMode;
    bool            m_bRenderVerticalLine;

    int             m_iStageType;       
    int             m_iEditType;        // 1Àº 1Ãþ ÇÃ·§Æû, 2´Â 2Ãþ ÇÃ·§Æû, 3Àº Àå¾Ö¹° 4´Â Á©¸® ¹èÄ¡ ´Ü°è 5´Â ´Ü°è ³¡
    int             m_iObstacleType;    
    int             m_iJellyType;       // 1. ±âº» Á©¸® (OBJID - JELLY), 2. °õÁ©¸® ( OBJID - GBJELLY) 3. ½Ç¹ö ÄÚÀÎ ( OBJID - SILVER_COIN ) 4. °ñµå ÄÚÀÎ ( OBJID - GOLD_COIN)
    int             m_iJellyTextType;
    float		    m_fStageWidth;
    float           m_fTileCX;
};

