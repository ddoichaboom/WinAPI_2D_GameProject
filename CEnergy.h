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
    float   m_fHealAmount;
};

