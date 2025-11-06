#pragma once
#include "CObj.h"

class CEffect;

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

    void    Activate_Boost(float fSpeed, float fDuration);
    void    Activate_Giant(float fTargetScale, float fDuration);
    void    Activate_Magnet(float fRadius, float fDuration);
    void    Activate_Change_Jelly(float fDuration);
    void    Activate_Change_Obstacle_To_Coin(float fDuration);
    void   Restore_Hp(float fHealAmount);
    
    bool    Is_Magnet_On() const { return m_bMagnetMode; }
    float   Get_Magnet_Radius() const { return m_fMagnetRadius; }
    bool    Is_Player_Can_Crash_Obstacle() const { return m_bBoostMode || m_bGiantMode; }
    bool    Is_Change_Jelly_Mode() const { return m_bChangeJellyMode; }
    bool    Is_Change_Obstacle_To_Coin_Mode() const { return m_bChangeObstacleToCoinMode; }
    bool    Is_Boost_Mode() const { return m_bBoostMode; }

    void    Collect_Alphabet(wchar_t alphabet);
    bool    Has_Alphabet(wchar_t alphabet) const;
    const map<wchar_t, bool>& Get_BonusTime_Alphabets() const { return m_mapBonusTimeAlphabet; }

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
    float           m_fBoostSpeed;          // 부스트 증가 속도
    float           m_fBoostEffectTimer;

    SCALE_STATE     m_eScaleState;
    float           m_fGiantTime;
    float           m_fCurrentScale;
    float           m_fTargetScale;
    float           m_fScaleSpeed;
    bool            m_bGiantMode;

    bool            m_bMagnetMode;
    float           m_fMagnetTime;
    float           m_fMagnetRadius;

    bool            m_bChangeJellyMode;
    float           m_fChangeJellyTime;

    bool            m_bChangeObstacleToCoinMode;
    float           m_fChangeObstacleTime;

    // 보너스 타임 알파벳 수집 (B, O, N, U, S, T, I, M, E)
    map<wchar_t, bool> m_mapBonusTimeAlphabet;

    CEffect*        m_pMagnetEffect;

};

