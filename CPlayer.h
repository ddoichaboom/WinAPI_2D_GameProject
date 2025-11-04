#pragma once
#include "CObj.h"

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

    enum SCALE_STATE { IDLE, SCALING_UP, GIANT, SCALING_DOWN };

public:
    void   Initialize() override;
    int	   Update(float deltaTime) override;
    void   Late_Update(float deltaTime) override;
    void   Render(HDC hDC) override;
    void   Release() override;

    void   On_Hit();
    void   Add_Score(int iScore);     
    void   Add_Coin(int iCoin);
    void   Take_Damage(float fDamage);
    const  PLAYERINFO* Get_PlayerInfo() const { return &m_tPlayerInfo; }
    bool   Is_Invincible() const { return m_bBoostMode || m_bBlinkMode || m_bGiantMode; }
    void   Time_Check(float deltaTime);

// 아이템에서 사용할 함수들
public:     
    void   Set_Boost_Mode(float fBoostTime) 
    { 
        m_bBoostMode = true;
        m_fBoostTime = fBoostTime;
    }

    void    Activate_Giant(float fTargetScale, float fDuration);
    void   Restore_Hp(float fHealAmount);
    

private:
    void	Collision_Border_Line();
    void    Key_Input();
    void    Offset(float deltaTime);
    void    Motion_Change();
    void    State_Check(float deltaTime);

private:
    int             m_iMaxJumpCount;
    int             m_iJumpCount;
    float           m_fJumpTimer;       // 점프 체공 시간 제어용 타이머

    STATE			m_ePreMotion;
    STATE			m_eCurMotion;
    PLAYERINFO      m_tPlayerInfo;
    

    bool            m_bWantJump;
    bool            m_bWantSlide;

    bool            m_bBlinkMode;
    bool            m_bDecelerated;
    float           m_fInvincibleTime;      // 무적 지속 시간 타이머 
    float           m_fDecelerationTime;    
    float           m_fBlinkTime;           // 깜박임효과 타이머 

    INFO            m_tRenderInfo;
    wstring         m_strJumpSoundPath;
    wstring         m_strSlideSoundPath;

    // 아이템에 사용할 변수들
    bool            m_bBoostMode;           //  부스트 모드 
    float           m_fBoostTime;          //  부스트 모드 지속시간 타이머
    SCALE_STATE     m_eScaleState;
    float           m_fGiantTime;
    float           m_fCurrentScale;
    float           m_fTargetScale;
    float           m_fScaleSpeed;
    bool            m_bGiantMode;
    
};

