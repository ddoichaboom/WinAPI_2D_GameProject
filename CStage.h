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
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    
public:
    ULONGLONG		m_currTime;			// Update 호출 시 시각 체크
    ULONGLONG		m_prevTime;			// 해당 프레임 생성시 시각 체크 
    float			m_deltaTime;		// 시간 차이 담는 변수

};

