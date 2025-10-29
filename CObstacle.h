#pragma once
#include "CObj.h"
class CObstacle :
    public CObj
{
public:
    CObstacle();
    virtual ~CObstacle();

public:
    void Initialize() override;
    int Update(float deltaTime) override;
    void Late_Update(float deltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;
};

