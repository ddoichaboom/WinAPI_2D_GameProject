#pragma once
#include "CItem.h"

class CBoost :
    public CItem
{
public:
    CBoost();
    virtual ~CBoost();

public:
    void    Initialize() override;
    void    Apply_Effect(CPlayer* pPlayer) override;
};

