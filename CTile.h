#pragma once
#include "CObj.h"
class CTile :
    public CObj
{
public:
    CTile();
    virtual ~CTile();



public:
    void Initialize() override;
    int Update(float deltaTime) override;
    void Late_Update(float deltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void    Set_Tile_State(bool bAppear) { m_bState = bAppear; };
    bool    Get_Tile_State() { return m_bState; };

private:
    bool    m_bState;

};

