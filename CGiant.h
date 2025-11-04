#pragma once
#include "CItem.h"

class CGiant :
    public CItem
{
public:
    CGiant();
    virtual ~CGiant();

public:
    void    Initialize() override;
    void    Apply_Effect(CPlayer* pPlayer) override;
};

