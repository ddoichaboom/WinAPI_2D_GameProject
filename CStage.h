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

private:
    void    Load_Chunk_Data(const TCHAR* pFilePath);        // 청크 로딩 함수 선언 

private:
    wstring     m_stageKey;
    float       m_fStageWidth;
    float       m_fWorldEndX;
    int         m_iCurrChunkIndex;

};

