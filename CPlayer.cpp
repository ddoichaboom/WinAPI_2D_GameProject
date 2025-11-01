#include "pch.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CTileMgr.h"
#include "CCollisionMgr.h"
#include "CDataMgr.h"


CPlayer::CPlayer()
    : m_iMaxJumpCount(2),
    m_ePreMotion(ST_END), m_eCurMotion(RUN), m_iJumpCount(0),
    m_bWantJump(false), m_bWantSlide(false), m_fJumpTimer(0.f),
    m_bInvincible(false), m_fInvincibleTime(0.f), m_fBlinkTime(0.f),
    m_fDecelerationTime(0.f), m_bDecelerated(false)
{
    ZeroMemory(&m_tPlayerInfo, sizeof(PLAYERINFO));
}

CPlayer::~CPlayer()
{
    Release();
}

void CPlayer::Initialize()
{
    m_fSpeed = 500.0f;
    m_fVx = m_fSpeed;
    m_fJumpSpeed = 550.f;
    m_bOnGround = true;
    m_bPrevOnGround = true;
    m_eRender = GAMEOBJECT;
    m_fVy = 0.f;
    m_fJumpTimer = 0.f;

    if (!lstrcmp(m_pFrameKey.c_str(), L"GINGER_BRAVE_COOKIE"))
    {
        m_tPlayerInfo.fHp = 100.f;
        m_tPlayerInfo.lScore = 0.f;
    }

}

int CPlayer::Update(float deltaTime)
{


    Key_Input();

    if (m_fVy < 0.f)
    {
        m_fJumpTimer -= deltaTime;
    }

    if (m_fJumpTimer <= 0.f)
    {
        m_fVy = m_fJumpSpeed;
    }

    if (m_bOnGround)
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
    if (m_bInvincible)
    {
        m_fInvincibleTime -= deltaTime;
        m_fBlinkTime += deltaTime;

        if (m_fInvincibleTime <= 0.f)
        {
            m_bInvincible = false;
            m_fInvincibleTime = 0.f;
            m_fBlinkTime = 0.f;
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


    State_Check(deltaTime);

    Motion_Change();

    Update_Rect(PLAYER);

    Offset(deltaTime);

    Move_Frame(deltaTime);

    m_bPrevOnGround = m_bOnGround;


}

void CPlayer::Render(HDC hDC)
{
    int 		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int 		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    int srcX = (int)m_tInfo.fCX * m_tFrame.iStart;
    int srcY = (int)m_tInfo.fCY * m_tFrame.iMotion;
    int srcW = (int)m_tInfo.fCX;
    int srcH = (int)m_tInfo.fCY;

    int dstX = (int)m_tRect.left + iScrollX;
    int dstY = (int)m_tRect.top + iScrollY;
    int dstW = (int)m_tInfo.fCX;
    int dstH = (int)m_tInfo.fCY;
    
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    if (m_bInvincible)
    {
        if (fmod(m_fBlinkTime, 0.2f) > 0.1f)
        {
            bf.SourceConstantAlpha = 128; // 반투명 
        }
        else
        {
            bf.SourceConstantAlpha = 255;
        }
    }

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



// 맵 제작 전 임시로 사용
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
    int     iOffSetX = 400;

    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();


    if (iOffSetX < m_tInfo.fX + iScrollX)
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(-m_fVx * deltaTime);
    }

}


void    CPlayer::Motion_Change()
{
    if (m_ePreMotion != m_eCurMotion)
    {
        switch (m_eCurMotion)
        {
        case RUN:
            m_tInfo.fHitCX = 120.f;
            m_tInfo.fHitCY = 133.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 0;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.05f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = true;    // 루프
            break;

        case JUMP:
            m_tInfo.fHitCX = 124.f;
            m_tInfo.fHitCY = 113.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 1;
            m_tFrame.iMotion = 1;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.00f;    // 락 없음
            m_tFrame.bLoop = true;
            break;

        case DOUBLE_JUMP_INTRO:
            m_tInfo.fHitCX = 120.f;
            m_tInfo.fHitCY = 145.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 2;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 100ms
            m_tFrame.stateLockRemainSec = 0.10f;    // 500ms
            m_tFrame.bLoop = false;
            break;

        case DOUBLE_JUMP_TURN:
            m_tInfo.fHitCX = 117.f;
            m_tInfo.fHitCY = 117.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 2;
            m_tFrame.iMotion = 3;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 100ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 500ms
            m_tFrame.bLoop = true;
            break;

        case DOUBLE_JUMP_OUTRO:
            m_tInfo.fHitCX = 110.f;
            m_tInfo.fHitCY = 120.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 4;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   // 100ms
            m_tFrame.stateLockRemainSec = 0.10f;    // 500ms
            m_tFrame.bLoop = false;
            break;

        case FALLING:
            m_tInfo.fHitCX = 77.f;
            m_tInfo.fHitCY = 144.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 5;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   
            m_tFrame.stateLockRemainSec = 0.0f;    
            m_tFrame.bLoop = false;
            break;


        case LANDING:
            m_tInfo.fHitCX = 113.f;
            m_tInfo.fHitCY = 112.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 6;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.10f;   
            m_tFrame.stateLockRemainSec = 0.025f;    
            m_tFrame.bLoop = false;    
            break;


        case SLIDE:
            m_tInfo.fHitCX = 168.f;
            m_tInfo.fHitCY = 66.f;
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
            m_tInfo.fHitCX = 122.f;
            m_tInfo.fHitCY = 140.f;
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 13;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.20f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = true;    // 루프
            break;

        case CLEAR:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 10;
            m_tFrame.iMotion = 14;
            m_tFrame.frameElapsedSec = 0.0f;
            m_tFrame.frameIntervalSec = 0.20f;   // 200ms
            m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
            m_tFrame.bLoop = true;    // 루프
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
            m_tFrame.bLoop = true;    // 루프
            break;

        }

        m_ePreMotion = m_eCurMotion;
    }
}

void    CPlayer::State_Check(float deltaTime)
{

    if (m_tFrame.stateLockRemainSec > 0.0f)
    {
        m_bWantJump = false;
        return;
    }

    // 0에 근접할 정도로 작은 소수를 판단하기 위함
    const float eps = 1e-3f;

    // 막 착지 했는지에 대한 여부 체크
    const bool bLanded = (!m_bPrevOnGround && m_bOnGround);

    if (m_bWantSlide && m_bOnGround && (fabsf(m_fVy) < eps))
    {
        m_eCurMotion = SLIDE;
        return;
    }

    if (m_bWantJump)
    {
        if (m_iMaxJumpCount > m_iJumpCount)
        {
            m_bOnGround = false;
            m_fVy = -m_fJumpSpeed;
            m_fJumpTimer = 0.35f;
            m_eCurMotion = (m_iJumpCount == 0) ? JUMP : DOUBLE_JUMP_INTRO;

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
        m_eCurMotion = LANDING;
        return;
    }

    if (fabsf(m_fVy) < eps)
    {
        m_eCurMotion = RUN;
        return;
    }

}

void    CPlayer::On_Hit()
{
    if (m_bInvincible)
        return;

    m_bInvincible = true;
    m_bDecelerated = true;
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
    m_tPlayerInfo.fHp -= fDamage;
    if (m_tPlayerInfo.fHp < 0.f)
        m_tPlayerInfo.fHp = 0.f;

    //TODO :  게임 오버 처리 로직 추가
}