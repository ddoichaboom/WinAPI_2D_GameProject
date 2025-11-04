#pragma once
#include "CObj.h"
#include "CPlayer.h"


class CItem :
    public CObj
{
public:
    CItem();
    virtual ~CItem();

public:
    virtual void        Apply_Effect(CPlayer* pPlayer) PURE;

public:
    virtual void        Initialize() override PURE;
    virtual int         Update(float deltaTime) override;
    virtual void        Late_Update(float deltaTime) override;
    virtual void        Render(HDC hDC) override;
    virtual void        Release() override;




};

