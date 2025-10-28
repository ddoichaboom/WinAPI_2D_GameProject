#pragma once
#include "CObj.h"
#include "CLine.h"

class CPlayer :
    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    enum STATE { 
        RUN,
        JUMP,
        DOUBLE_JUMP_INTRO, DOUBLE_JUMP_TURN, DOUBLE_JUMP_OUTRO,
        FALLING, LANDING,
        SLIDE, HIT,
        BONUS_TIME_INTRO, BONUS_TIME_UP, BONUS_TIME_DOWN, BONUS_TIME_OUTRO,
        BOOST, CLEAR, EXHAUST, DEAD,
        ST_END };

public:
    void Initialize() override;
    int	 Update(float deltaTime) override;
    void Late_Update(float deltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    void	Rect_Col_Movement();
    void	Collision_Border_Line();
    void    Key_Input();
    void    Offset(float deltaTime);
    void    Motion_Change();
    void    State_Check(float deltaTime);

private:
    CLine*          m_pTargetLine;
    CLine*          m_pVerticalLine;
    int             m_iMaxJumpCount;
    int             m_iJumpCount;

    STATE			m_ePreMotion;
    STATE			m_eCurMotion;
    PLAYERINFO      m_tPlayerInfo;
    

    bool            m_bWantJump;
    bool            m_bWantSlide;



};

