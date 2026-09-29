#pragma once
#include "CScene.h"
class CEdit :
    public CScene
{
public:
    CEdit();
    virtual ~CEdit();

public:
    enum EDIT_TYPE { 
        EDIT_FIRST_PLATFORM,
        EDIT_SECOND_PLATFORM,
        EDIT_OBSTACLE,
        EDIT_JELLY,
        EDIT_ITEM,
        EDIT_TYPE_END};

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
    const TCHAR*    m_pTileFrameKey;        // 첫번째 플랫폼 전용 프레임 키



    POINT			m_tCursor;
    bool            m_bRenderVerticalLine;
    bool            m_bSceneLeave;

    int             m_iStageType; 
    int             m_iPrevStageType;
    int             m_iEditType;        // 1은 1층 플랫폼, 2는 2층 플랫폼, 3은 장애물 4는 젤리 배치 단계 5는 단계 끝
    int             m_iObstacleType;    
    int             m_iJellyType;       // 1. 기본 젤리 (OBJID - JELLY), 2. 곰젤리 ( OBJID - GBJELLY) 3. 실버 코인 ( OBJID - SILVER_COIN ) 4. 골드 코인 ( OBJID - GOLD_COIN)
    int             m_iJellyTextType;
    int             m_iItemType;        // 0. item_energy 1. item_energy_big, 2. item_boost 3. item_giant ...
    float           m_fTileCX;
    int             m_iCurrentChunkIndex;       // 현재 편집 중인 청크 인덱스 (0부터 시작)
    float           m_fBgTileWidth;             // 배경 타일 너비 (청크 너비)
    float           m_fEditorScrollX;           // 에디터 누적 스크롤 
};

