#pragma once
#include "CItem.h"

class CEnergy :
    public CItem
{
public:
    CEnergy();
    virtual ~CEnergy();

public:
    void    Initialize()        override;
    void    Apply_Effect(CPlayer* pPlayer) override;

private:
    ITEM_TYPE       m_eItemType;
};

