#include "pch.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CTileMgr.h"
#include "CCollisionMgr.h"
#include "CDataMgr.h"
#include "CSoundMgr.h"
#include "CEffect.h"
#include "CEffectMgr.h"
#include "CAbstractFactory.h"
#include "Factory.h"


CPlayer::CPlayer()
    : m_iMaxJumpCount(2),
    m_ePreMotion(ST_END), m_eCurMotion(RUN), m_iJumpCount(0),
    m_bWantJump(false), m_bWantSlide(false), m_fJumpTimer(0.f),
    m_fInvincibleTime(0.f), m_fBlinkTime(0.f),
    m_fDecelerationTime(0.f), m_bDecelerated(false),
    m_bBlinkMode(false), m_fBoostTime(0.f), m_bBoostMode(false),
    m_fTargetScale(1.f), m_fGiantTime(0.f),
    m_fCurrentScale(1.f), m_fScaleSpeed(2.f), m_eScaleState(IDLE),
    m_bGiantMode(false), m_strJumpSoundPath(L""), m_strSlideSoundPath(L""),
    m_bMagnetMode(false), m_fMagnetTime(0.f), m_fMagnetRadius(0.f),
    m_fBoostSpeed(0.f), m_bChangeJellyMode(false), m_bChangeObstacleToCoinMode(false),
    m_fChangeJellyTime(0.f), m_fChangeObstacleTime(0.f), m_pMagnetEffect(nullptr),
    m_fBoostEffectTimer(0.f), m_fHpDecreaseTimer(0.f),
    m_bClearSequence(false), m_fClearTimer(0.f), m_fClearDuration(0.f), m_fOriginalSpeed(0.f)
{
    ZeroMemory(&m_tPlayerInfo, sizeof(PLAYERINFO));
    ZeroMemory(&m_tRenderInfo, sizeof(INFO));
}

CPlayer::~CPlayer()
{
    Release();
}

void CPlayer::Initialize()
{
    m_fSpeed = 650.0f;
    m_fVx = m_fSpeed;
    m_fJumpSpeed = 625.f;
    m_bOnGround = true;
    m_bPrevOnGround = true;
    m_eRender = GAMEOBJECT;
    m_fVy = 0.f;
    m_fJumpTimer = 0.f;

    if (!lstrcmp(m_pFrameKey.c_str(), L"Cookie0001"))
    {
        m_tPlayerInfo.fMaxHp = 150.f;
        m_tPlayerInfo.fHp = m_tPlayerInfo.fMaxHp;
        m_tPlayerInfo.lScore = 0.f;
    }

    m_tRenderInfo = m_tInfo;

    m_strJumpSoundPath = L"./Sound/Effect/";
    m_strJumpSoundPath += Get_FrameKey();
    m_strJumpSoundPath += L"_Jump.wav";

    m_strSlideSoundPath = L"./Sound/Effect/";
    m_strSlideSoundPath += Get_FrameKey();
    m_strSlideSoundPath += L"_Slide.wav";


}

int CPlayer::Update(float deltaTime)
{
    //Time_Check(deltaTime);

    Key_Input();

    if (m_fVy < 0.f)
    {
        m_fJumpTimer -= deltaTime;
    }

    if (m_fJumpTimer <= 0.f)
    {
        m_fVy = m_fJumpSpeed;
    }
    //if ((fabsf(m_fVy) < eps) && m_fVy < 0.f)
    //{
    //    m_fVy = m_fJumpSpeed;
    //}

    if (!m_bOnGround)
    {
        m_fVy += GC * deltaTime;
    }
    else if (m_bOnGround)
    {
        m_iJumpCount = 0;
        m_fVy = 0;
    }

    m_tInfo.fY += m_fVy * deltaTime;
    m_tInfo.fX += m_fVx * deltaTime;
    
    
    Update_Rect(PLAYER);

    m_bOnGround = false;


    return OBJ_NOEVENT;
}

void CPlayer::Late_Update(float deltaTime) 
{
    if (m_eCurMotion == DEAD)
    {
        Motion_Change();

        Update_Rect(PLAYER);
        Offset(deltaTime);

        Move_Frame(deltaTime);

        m_bPrevOnGround = m_bOnGround;


        // 사망 애니메이션이 끝나면 게임 오버
        if (!m_tFrame.bLoop && (m_tFrame.iStart == m_tFrame.iEnd))
        {
            // TODO : 게임 오버 씬 전환 
            // Set_Dead();      // 필요하면 적용
        }
        return;
    }

    // 클리어 시퀀스 진행 중
    if (m_bClearSequence)
    {
        Time_Check(deltaTime);

        Motion_Change();
        Update_Rect(PLAYER);
        Offset(deltaTime);
        Move_Frame(deltaTime);

        m_bPrevOnGround = m_bOnGround;
        return;
    }

    Time_Check(deltaTime);

    const float prevBottom = m_tInfo.fY + (m_tInfo.fCY * 0.5f);

    if ((m_eScaleState != SCALE_STATE::IDLE) && m_bGiantMode)
    {

        // 커지는 중
        if (m_eScaleState == SCALE_STATE::SCALING_UP)
        {
            m_fCurrentScale += m_fScaleSpeed * deltaTime;
            if (m_fCurrentScale >= m_fTargetScale)
            {
                m_fCurrentScale = m_fTargetScale;
                m_eScaleState = SCALE_STATE::GIANT;
            }
        }
        // 거대화 지속 
        else if (m_eScaleState == SCALE_STATE::GIANT)
        {
            m_fGiantTime -= deltaTime;
            if (m_fGiantTime <= 0.f)
            {
                m_fGiantTime = 0.f;
                m_eScaleState = SCALE_STATE::SCALING_DOWN;

            }
        }
        // 작아지는 중
        else if (m_eScaleState == SCALE_STATE::SCALING_DOWN)
        {
            m_fCurrentScale -= m_fScaleSpeed * deltaTime;
            if (m_fCurrentScale <= 1.f)
            {
                m_fCurrentScale = 1.f;
                m_eScaleState = SCALE_STATE::IDLE;
                m_bGiantMode = false;
            }
        }

    }

    State_Check(deltaTime);

    Motion_Change();

    m_tInfo.fCX = m_tRenderInfo.fCX;
    m_tInfo.fCY = m_tRenderInfo.fCY;
    m_tInfo.fHitCX = m_tRenderInfo.fHitCX;
    m_tInfo.fHitCY = m_tRenderInfo.fHitCY;

    if (m_fCurrentScale != 1.f)
    {
        m_tInfo.fCX     *= m_fCurrentScale;
        m_tInfo.fCY     *= m_fCurrentScale;
        m_tInfo.fHitCX  *= m_fCurrentScale;
        m_tInfo.fHitCY  *= m_fCurrentScale;
    }

    if ((m_eScaleState != SCALE_STATE::IDLE) && m_bGiantMode)
    {
        const float newBottom   = m_tInfo.fY + (m_tInfo.fCY * 0.5f);
        const float diff        = prevBottom - newBottom;
        m_tInfo.fY += diff;
    }


    Update_Rect(PLAYER);

    Offset(deltaTime);

    Move_Frame(deltaTime);

    m_bPrevOnGround = m_bOnGround;

}

void CPlayer::Render(HDC hDC)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    int srcX = (int)m_tRenderInfo.fCX * m_tFrame.iStart;
    int srcY = (int)m_tRenderInfo.fCY * m_tFrame.iMotion;
    int srcW = (int)m_tRenderInfo.fCX;
    int srcH = (int)m_tRenderInfo.fCY;

    int dstX = (int)m_tRect.left + iScrollX;
    int dstY = (int)m_tRect.top + iScrollY;
    int dstW = (int)m_tInfo.fCX;
    int dstH = (int)m_tInfo.fCY;
    
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    if (m_bBlinkMode && fmod(m_fBlinkTime, 0.2f) > 0.1f)
        bf.SourceConstantAlpha = 128; // 반투명 
        

    HDC hPlayerDC = CBmpMgr::Get_Instance()->Find_Image(Get_FrameKey());

     //히트박스 체크용
    //Rectangle(
    //    hDC,
    //    (int)m_tHitRect.left + iScrollX,
    //    (int)m_tHitRect.top,
    //    (int)m_tHitRect.right + iScrollX,
    //    (int)m_tHitRect.bottom);

    AlphaBlend(hDC,
        dstX, dstY,
        dstW, dstH,
        hPlayerDC,
        srcX, srcY,
        srcW, srcH,
        bf);
}

void CPlayer::Release()
{

}


// TODO : 부스트모드 true일 때 타일 - 1층 플랫폼의 HitRect.top을 기준점으로 안떨어지게 설정해야함 
void CPlayer::Collision_Border_Line()
{

    if (m_tHitRect.bottom >= BOUNDARY_BOTTOM )
    {
        m_tInfo.fY = BOUNDARY_BOTTOM - (m_tInfo.fCY / 2.f);
        Set_Hit_Pos(m_tInfo.fX, m_tInfo.fY + (m_tInfo.fCY - m_tInfo.fHitCY) / 2.f);
        m_bOnGround = true;
        m_fVy = 0;
    }
}


void CPlayer::Key_Input()
{

    if (CKeyMgr::Get_Instance()->Key_Down('J'))
    {
        m_bWantJump = true;
    }
    
    m_bWantSlide = CKeyMgr::Get_Instance()->Key_Pressing('S'); 

    
}

void CPlayer::Offset(float deltaTime)
{
    //int     iOffSetX = 400;

    //int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();


    //if (iOffSetX  < m_tInfo.fX + iScrollX )
    //{
    //    CScrollMgr::Get_Instance()->Set_ScrollX(-m_fVx * deltaTime);
    //}

    CScrollMgr::Get_Instance()->FollowX(m_tInfo.fX, 400.f);

}


void    CPlayer::Motion_Change()
{
    if (m_ePreMotion != m_eCurMotion)
    {
        switch (m_eCurMotion)
        {
        case RUN:
            m_tRenderInfo.fHitCX = 120.f;
            m_tRenderInfo.fHitCY = 133.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 0;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.025f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = true;    // 루프
            break;

        case JUMP:
            m_tRenderInfo.fHitCX = 124.f;
            m_tRenderInfo.fHitCY = 113.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 1;
            m_tFrame.iMotion = 1;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.00f;    // 락 없음
            m_tFrame.bLoop = true;
            break;

        case DOUBLE_JUMP_INTRO:
            m_tRenderInfo.fHitCX = 120.f;
            m_tRenderInfo.fHitCY = 145.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 2;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 100ms
            m_tFrame.stateLockRemainSec = 0.10f;    // 500ms
            m_tFrame.bLoop = false;
            break;

        case DOUBLE_JUMP_TURN:
            m_tRenderInfo.fHitCX = 117.f;
            m_tRenderInfo.fHitCY = 117.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 2;
            m_tFrame.iMotion = 3;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 100ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 500ms
            m_tFrame.bLoop = true;
            break;

        case DOUBLE_JUMP_OUTRO:
            m_tRenderInfo.fHitCX = 110.f;
            m_tRenderInfo.fHitCY = 120.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 4;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 100ms
            m_tFrame.stateLockRemainSec = 0.10f;    // 500ms
            m_tFrame.bLoop = false;
            break;

        case FALLING:
            m_tRenderInfo.fHitCX = 77.f;
            m_tRenderInfo.fHitCY = 144.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 5;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   
            m_tFrame.stateLockRemainSec = 0.0f;    
            m_tFrame.bLoop = false;
            break;


        case LANDING:
            m_tRenderInfo.fHitCX = 113.f;
            m_tRenderInfo.fHitCY = 112.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 6;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   
            m_tFrame.stateLockRemainSec = 0.025f;    
            m_tFrame.bLoop = false;    
            break;


        case SLIDE:
            m_tRenderInfo.fHitCX = 168.f;
            m_tRenderInfo.fHitCY = 66.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 1;
            m_tFrame.iMotion = 7;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.20f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = true;    // 루프


            break;


        case HIT:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 4;
            m_tFrame.iMotion = 8;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.00f;    // 500ms
            m_tFrame.bLoop = false;    // 루프
            break;


        case BONUS_TIME_INTRO:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 4;
            m_tFrame.iMotion = 9;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 100ms
            m_tFrame.stateLockRemainSec = 0.50f;    
            m_tFrame.bLoop = false;    // 루프

            break;

        case BONUS_TIME_UP:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 2;
            m_tFrame.iMotion = 10;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 300ms
            m_tFrame.bLoop = false;    
            break;

        case BONUS_TIME_DOWN:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 1;
            m_tFrame.iMotion = 11;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.20f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = true;    // 루프
            break;

        case BONUS_TIME_OUTRO:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 2;
            m_tFrame.iMotion = 12;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.20f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = true;    // 루프
            break;

        case BOOST:
            m_tRenderInfo.fHitCX = 122.f;
            m_tRenderInfo.fHitCY = 140.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 13;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = true;    // 루프
            break;

        case CLEAR:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 10;
            m_tFrame.iMotion = 14;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.30f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.5f;    // 락 없음
            m_tFrame.bLoop = false;    // 루프
            break;

        case EXHAUST:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 15;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.20f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = true;    // 루프
            break;

        case DEAD:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 4;
            m_tFrame.iMotion = 16;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.20f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = false;    // 루프
            break;

        }

        m_ePreMotion = m_eCurMotion;
    }
}

void    CPlayer::State_Check(float deltaTime)
{
    // 클리어 시퀀스 중에는 상태 변경 금지
    if (m_bClearSequence)
    {
        return;
    }

    if (m_tFrame.stateLockRemainSec > 0.0f)
    {
        //m_bWantJump = false;
        return;
    }

    const float eps = 1e-3f;

    // 막 착지 했는지에 대한 여부 체크
    const bool bLanded = (!m_bPrevOnGround && m_bOnGround);

    if (m_bWantSlide && m_bOnGround && (fabsf(m_fVy) < eps))
    {
        if (m_eCurMotion != SLIDE)
        {
            m_eCurMotion = SLIDE;
            CSoundMgr::Get_Instance()->PlaySound(m_strSlideSoundPath.c_str(), SOUND_PLAYER_SLIDE, 0.8f);
        }
        return;
    }

    if (m_bWantJump)
    {
        if (m_iMaxJumpCount > m_iJumpCount)
        {
            m_bOnGround = false;
            m_fVy = -m_fJumpSpeed;
            m_fJumpTimer = JUMP_APEX_TIME;
            m_eCurMotion = (m_iJumpCount == 0) ? JUMP : DOUBLE_JUMP_INTRO;

            if (!m_bGiantMode)
                CSoundMgr::Get_Instance()->PlaySound(m_strJumpSoundPath.c_str(), SOUND_PLAYER_JUMP, 0.8f);
            else
                CSoundMgr::Get_Instance()->PlaySound(L"./Sound/Effect/Giant_Jump.wav", SOUND_PLAYER_JUMP, 0.8f);


            ++m_iJumpCount;
        }
        m_bWantJump = false;
        return;
    }

    if (m_eCurMotion == DOUBLE_JUMP_INTRO)
    {
        m_eCurMotion = DOUBLE_JUMP_TURN;
        return;
    }

    if ((m_eCurMotion == DOUBLE_JUMP_TURN))
    {
        if (m_fVy > eps)
            m_eCurMotion = DOUBLE_JUMP_OUTRO;

        return;
    }

    if ((m_eCurMotion != JUMP) && (!m_bOnGround))
    {
        if (m_fVy > eps)
            m_eCurMotion = FALLING;

        return;
    }

    if (bLanded && ((m_eCurMotion == FALLING) || (m_eCurMotion == JUMP)))
    {
        if (m_bGiantMode)
            CSoundMgr::Get_Instance()->PlaySound(L"./Sound/Effect/Giant_Land.wav", SOUND_LAND, 0.8f);
        m_eCurMotion = LANDING;
        return;
    }

    if (fabsf(m_fVy) < eps)
    {
        if (m_bBoostMode)
            m_eCurMotion = BOOST;
        else
            m_eCurMotion = RUN;
        return;
    }

}

void    CPlayer::On_Hit()
{
    if (Is_Invincible())
        return;

    m_bDecelerated = true;
    m_bBlinkMode = true;
    m_fInvincibleTime = 2.0f;       
    m_fDecelerationTime = 1.f;
    m_eCurMotion = HIT;

}

void    CPlayer::Add_Score(int iScore)
{
    m_tPlayerInfo.lScore += iScore;
}

void    CPlayer::Add_Coin(int iCoin)
{
    m_tPlayerInfo.lCoin += iCoin;
}

void    CPlayer::Take_Damage(float fDamage)
{
    if (m_eCurMotion == DEAD)
        return;

    m_tPlayerInfo.fHp -= fDamage;

    if (m_tPlayerInfo.fHp <= 0.f)
    {
        m_tPlayerInfo.fHp = 0.f;
        m_eCurMotion = DEAD;
        m_fVx = 0.f;
    }
}

void   CPlayer::Restore_Hp(float fHealAmount)
{
    m_tPlayerInfo.fHp += fHealAmount;

    if (m_tPlayerInfo.fHp > m_tPlayerInfo.fMaxHp)
        m_tPlayerInfo.fHp = m_tPlayerInfo.fMaxHp;
}

// 시간 체크해주는 기능들 모아놓을 함수 
void   CPlayer::Time_Check(float deltaTime)
{
    if (m_bBlinkMode)
    {
        m_fInvincibleTime -= deltaTime;
        m_fBlinkTime += deltaTime;

        if (m_fInvincibleTime <= 0.f)
        {
            m_bBlinkMode = false;
            m_fInvincibleTime = 0.f;
            m_fBlinkTime = 0.f;
        }
    }

    if (m_bBoostMode)
    {
        m_fBoostTime -= deltaTime;
        m_fBoostEffectTimer += deltaTime;

        if (m_fBoostEffectTimer >= 0.1f)        // 1초에 10개
        {
            CEffectMgr::Get_Instance()->Add_Effect(
                static_cast<CEffect*>(Create_Effect(m_tInfo.fHitX, m_tInfo.fHitY, L"EFFECT_ITEM_BOOST")));
            m_fBoostEffectTimer = 0.f;
        }


        if (m_fBoostTime > 0.f)
        {
            m_fVx = m_fSpeed + m_fBoostSpeed;
        }
        else
        {
            m_bBoostMode = false;
            m_fVx = m_fSpeed;
            m_fBoostTime = 0.f;
            m_fBoostEffectTimer = 0.f;
        }
    }

    if (m_bDecelerated)
    {
        m_fDecelerationTime -= deltaTime;
        m_fVx = m_fSpeed - 150.f;

        if (m_fDecelerationTime <= 0.f)
        {
            m_bDecelerated = false;
            m_fDecelerationTime = 0.f;
            m_fVx = m_fSpeed;
        }
    }

    if (m_bMagnetMode)
    {
        m_fMagnetTime -= deltaTime;

        if (!m_pMagnetEffect)
        {
            m_pMagnetEffect = static_cast<CEffect*>(
                Create_Effect(m_tInfo.fHitX, m_tInfo.fHitY, L"EFFECT_ITEM_MAGNET"));
            CEffectMgr::Get_Instance()->Add_Effect(m_pMagnetEffect);
        }
        else
        {
            m_pMagnetEffect->Set_Pos(m_tInfo.fHitX, m_tInfo.fHitY);
        }

        if (m_fMagnetTime <= 0.f)
        {
            m_bMagnetMode = false;
            m_fMagnetTime = 0.f;
            m_fMagnetRadius = 0.f;

            if (m_pMagnetEffect)
            {
                m_pMagnetEffect->Set_Dead();
                m_pMagnetEffect = nullptr;
            }
        }
    }

    if (m_bChangeJellyMode)
    {
        m_fChangeJellyTime -= deltaTime;

        if (m_fChangeJellyTime <= 0.f)
        {
            m_bChangeJellyMode = false;
            m_fChangeJellyTime = 0.f;
        }
    }

    if (m_bChangeObstacleToCoinMode)
    {
        m_fChangeObstacleTime -= deltaTime;

        if (m_fChangeObstacleTime <= 0.f)
        {
            m_bChangeObstacleToCoinMode = false;
            m_fChangeObstacleTime = 0.f;
        }
    }

    // 클리어 시퀀스 처리
    if (m_bClearSequence)
    {
        m_fClearTimer += deltaTime;

        // 속도 감소 보간 (선형)
        float progress = m_fClearTimer / m_fClearDuration;
        progress = min(progress, 1.0f);  // 1.0 초과 방지

        // 원래 속도에서 0으로 점진적 감소
        m_fVx = m_fOriginalSpeed * (1.0f - progress);

        // 완전히 멈추면 CLEAR 모션
        if (progress >= 1.0f)
        {
            m_fVx = 0.f;
            m_eCurMotion = CLEAR;
        }
    }

    // 체력 감소 시스템 (일정 시간마다 체력 감소)
    m_fHpDecreaseTimer += deltaTime;

    if (m_fHpDecreaseTimer >= HP_DECREASE_INTERVAL)
    {
        Take_Damage(HP_DECREASE_AMOUNT);
        m_fHpDecreaseTimer = 0.f;
    }
}

void    CPlayer::Activate_Boost(float fSpeed, float fDuration)
{
    if (m_bBoostMode)
    {
        m_fBoostTime += fDuration;
        return;
    }
    else
    {
        m_bBoostMode = true;
        m_fBoostTime = fDuration;
        m_fBoostSpeed = fSpeed;
        return;
    }
}

void    CPlayer::Activate_Giant(float fTargetScale, float fDuration)
{
    if (m_eScaleState == SCALE_STATE::IDLE)
    {
        m_bGiantMode = true;
        m_eScaleState = SCALE_STATE::SCALING_UP;
        m_fTargetScale = fTargetScale;
        m_fGiantTime = fDuration;

        CSoundMgr::Get_Instance()->PlaySound(L"./Sound/Effect/Giant.mp3", SOUND_EFFECT, 0.8f);
    }
    else if (m_eScaleState == SCALE_STATE::GIANT)
    {
        m_fGiantTime += fDuration;
    }
}

void    CPlayer::Activate_Magnet(float fRadius, float fDuration)
{
    if (m_bMagnetMode)
    {
        // 이미 자석 상태라면 지속 시간만 증가
        m_fMagnetTime += fDuration;
        return;
    }
    else
    {
        m_bMagnetMode = true;
        m_fMagnetRadius = fRadius;
        m_fMagnetTime = fDuration;

        return;
    }
}

void CPlayer::Activate_Change_Jelly(float fDuration)
{
    if (m_bChangeJellyMode)
        m_fChangeJellyTime += fDuration;
    else
    {
        m_bChangeJellyMode = true;
        m_fChangeJellyTime = fDuration;
    }
}

void CPlayer::Activate_Change_Obstacle_To_Coin(float fDuration)
{
    if (m_bChangeObstacleToCoinMode)
        m_fChangeObstacleTime += fDuration;
    else
    {
        m_bChangeObstacleToCoinMode = true;
        m_fChangeObstacleTime = fDuration;
    }
}

void CPlayer::Collect_Alphabet(wchar_t alphabet)
{
    if (alphabet == L'B' || alphabet == L'O' || alphabet == L'N' ||
        alphabet == L'U' || alphabet == L'S' || alphabet == L'T' ||
        alphabet == L'I' || alphabet == L'M' || alphabet == L'E')
    {
        m_mapBonusTimeAlphabet[alphabet] = true;
    }
}

bool CPlayer::Has_Alphabet(wchar_t alphabet) const
{
    auto it = m_mapBonusTimeAlphabet.find(alphabet);
    return it != m_mapBonusTimeAlphabet.end() && it->second;

}

void    CPlayer::Activate_Clear_Sequence(float fDuration)
{
    m_bClearSequence = true;
    m_fClearTimer = 0.f;
    m_fClearDuration = fDuration;
    m_fOriginalSpeed = m_fVx;  // 현재 속도 저장
}