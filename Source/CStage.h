#pragma once
#include "CScene.h"
class CStage :
    public CScene
{
public:
    CStage();
    virtual ~CStage();

public:
    void Initialize() override;
    void Update(float fDeltaTime) override;
    void Late_Update(float fDeltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void            Switch_Stage(int nextStageID);
    STAGE_CONFIG    Get_Current_Stage()  const { return m_currentStage; }
    void            Request_Stage_Switch(int nextStageID);

private:
    void    Render_Background_Tiled(HDC hDC);
    void    Load_Next_Chunk();
    void    Initialize_Stage_Config(int stageID);

private:

    STAGE_CONFIG    m_currentStage;
    float           m_fWorldEndX;
    float           m_fBgTileWidth;
    
    bool            m_bPendingStageSwitch;      // 전환 대기 플래그
    int             m_iNextStageID;             // 전환할 스테이지 ID

    int             m_iCurrChunkIndex;

};

