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


};

