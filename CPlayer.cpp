#include "pch.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CLineMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CPlayer::CPlayer()
    : m_pTargetLine(nullptr), m_pVerticalLine(nullptr), m_iMaxJumpCount(2),
    m_ePreMotion(ST_END), m_eCurMotion(RUN), m_iJumpCount(0),
    m_bWantJump(false), m_bWantSlide(false), m_bPrevOnGround(false)
{
    ZeroMemory(&m_tPlayerInfo, sizeof(PLAYERINFO));
}

CPlayer::~CPlayer()
{
    Release();
}

void CPlayer::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/GingerBrave_Cookie.png", L"GINGER_BRAVE_COOKIE");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/GingerBrave_Blink.bmp", L"GINGER_BRAVE_COOKIE_BLINK");
    // 용감한 쿠키 - 블링크 버전 Motion_Change 추후 수정 

    m_pFrameKey = L"GINGER_BRAVE_COOKIE";

    m_tInfo = { 300.f, 300.f, 364.f, 364.f };
    m_fSpeed = 400.f;
    m_fVx = m_fSpeed;
    m_fJumpSpeed = 700.f;
    m_bOnGround = false;
    m_bPrevOnGround = false;
    m_eRender = GAMEOBJECT;
}

int CPlayer::Update(float deltaTime)
{
    Key_Input();

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


    __super::Update_Rect();


    return OBJ_NOEVENT;
}

void CPlayer::Late_Update(float deltaTime) 
{

    Rect_Col_Movement();

    Collision_Border_Line();

    State_Check(deltaTime);


    Motion_Change();

    Offset(deltaTime);

    Move_Frame(deltaTime);

    m_bPrevOnGround = m_bOnGround;
}

void CPlayer::Render(HDC hDC)
{
    int 		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int 		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // 버튼 스프라이트 시트의 한 프레임 영역 계산
    int srcX = (int)m_tInfo.fCX * m_tFrame.iStart;
    int srcY = (int)m_tInfo.fCY * m_tFrame.iMotion;
    int srcW = (int)m_tInfo.fCX;
    int srcH = (int)m_tInfo.fCY;

    int dstX = (int)m_tRect.left + iScrollX;
    int dstY = (int)m_tRect.top + iScrollY;
    int dstW = (int)m_tInfo.fCX;
    int dstH = (int)m_tInfo.fCY;;

    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    HDC hPlayerDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


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

void CPlayer::Rect_Col_Movement()
{
    if (m_fVy >= 0)
    {
        m_pTargetLine = CLineMgr::Get_Instance()->Line_Movement_Rect(&m_tInfo);

        // tfTilt, tfYInter, tfRadian 등
        if (m_pTargetLine)
        {
            const auto& L = m_pTargetLine->Get_Info(); // tfTilt, tfYInter, tfRadian 등

            // tfTilt, tfYInter, tfRadian 등
            float fDistance =
                sqrtf(((L.tfTilt * m_tInfo.fX - m_tInfo.fY + L.tfYInter) *
                    (L.tfTilt * m_tInfo.fX - m_tInfo.fY + L.tfYInter)) /
                    (L.tfTilt * L.tfTilt + 1.f));

            m_bOnGround = true;

            if (L.tfTilt == 0)
            {
                m_fVy = 0;
            }

            if (fabsf(L.tfTilt) == 1)
            {
                m_tInfo.fX = m_tInfo.fX + (INV_SQRT2 * m_tInfo.fCX * cosf(L.tfRadian) - fDistance * cosf(L.tfRadian));
                m_tInfo.fY = m_tInfo.fY - (INV_SQRT2 * m_tInfo.fCX * sinf(L.tfRadian) - fDistance * sinf(L.tfRadian));
            }
            else
            {
                m_tInfo.fX = m_tInfo.fX + ((INV_SQRT2 * m_tInfo.fCX * INV_SQRT2) -
                    (fDistance / sinf((PI / 4.0f + L.tfRadian)) * INV_SQRT2));
                m_tInfo.fY = m_tInfo.fY - ((INV_SQRT2 * m_tInfo.fCY * INV_SQRT2) -
                    (fDistance / sinf((PI / 4.0f + L.tfRadian)) * INV_SQRT2));
            }
        }
        else
            m_bOnGround = false;

        m_pTargetLine = nullptr;

    }
}

void CPlayer::Collision_Border_Line()
{
    if (m_tInfo.fX <= BOUNDARY_LEFT + m_tInfo.fCX / 2)
    {
        m_tInfo.fX = BOUNDARY_LEFT + m_tInfo.fCX / 2;
        m_fVx = 0;
    }

    if (m_tInfo.fY <= BOUNDARY_TOP + m_tInfo.fCY / 2)
    {
        m_tInfo.fY = BOUNDARY_TOP + m_tInfo.fCY / 2;
        m_fVy = 0;
    }

    if (m_tInfo.fX >= BOUNDARY_RIGHT - m_tInfo.fCX / 2)
    {
        m_tInfo.fX = BOUNDARY_RIGHT - m_tInfo.fCX / 2;
        m_fVx = 0;
    }

    if (m_tInfo.fY >= BOUNDARY_BOTTOM - m_tInfo.fCY / 2)
    {
        m_tInfo.fY = BOUNDARY_BOTTOM - m_tInfo.fCY / 2;
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
    int     iOffSetX = 300;

    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();


    if (iOffSetX < m_tInfo.fX + iScrollX)
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed * deltaTime);
    }

}


void    CPlayer::Motion_Change()
{
    if (m_pFrameKey == L"GINGER_BRAVE_COOKIE")
    {
        if (m_ePreMotion != m_eCurMotion)
        {
            switch (m_eCurMotion)
            {
            case RUN:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 3;
                m_tFrame.iMotion = 0;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.20f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
                m_tFrame.bLoop = true;    // 루프
                break;

            case JUMP:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 1;
                m_tFrame.iMotion = 1;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.20f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.00f;    // 락 없음
                m_tFrame.bLoop = true;
                break;


            case DOUBLE_JUMP:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 4;
                m_tFrame.iMotion = 2;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.10f;   // 100ms
                m_tFrame.stateLockRemainSec = 0.50f;    // 500ms
                m_tFrame.bLoop = false;    // 루프
                break;


            case FALLING:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 0;
                m_tFrame.iMotion = 3;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.10f;   
                m_tFrame.stateLockRemainSec = 0.0f;    
                m_tFrame.bLoop = true;
                break;


            case LANDING:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 0;
                m_tFrame.iMotion = 4;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.10f;   
                m_tFrame.stateLockRemainSec = 0.10f;    
                m_tFrame.bLoop = false;    
                break;


            case SLIDE:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 1;
                m_tFrame.iMotion = 5;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.20f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
                m_tFrame.bLoop = true;    // 루프

                break;


            case HIT:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 4;
                m_tFrame.iMotion = 6;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.10f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.50f;    // 500ms
                m_tFrame.bLoop = false;    // 루프
                break;


            case BONUS_TIME_INTRO:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 4;
                m_tFrame.iMotion = 7;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.10f;   // 100ms
                m_tFrame.stateLockRemainSec = 0.50f;    
                m_tFrame.bLoop = false;    // 루프

                break;

            case BONUS_TIME_UP:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 2;
                m_tFrame.iMotion = 8;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.10f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.30f;    // 300ms
                m_tFrame.bLoop = false;    
                break;

            case BONUS_TIME_DOWN:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 1;
                m_tFrame.iMotion = 9;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.20f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
                m_tFrame.bLoop = true;    // 루프
                break;

            case BONUS_TIME_OUTRO:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 2;
                m_tFrame.iMotion = 10;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.20f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
                m_tFrame.bLoop = true;    // 루프
                break;

            case BOOST:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 3;
                m_tFrame.iMotion = 11;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.20f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
                m_tFrame.bLoop = true;    // 루프
                break;

            case CLEAR:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 10;
                m_tFrame.iMotion = 12;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.20f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
                m_tFrame.bLoop = true;    // 루프
                break;

            case EXHAUST:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 3;
                m_tFrame.iMotion = 13;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.20f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
                m_tFrame.bLoop = true;    // 루프
                break;

            case DEAD:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 4;
                m_tFrame.iMotion = 14;
                m_tFrame.frameElapsedSec = 0.0f;
                m_tFrame.frameIntervalSec = 0.20f;   // 200ms
                m_tFrame.stateLockRemainSec = 0.0f;    // 락 없음
                m_tFrame.bLoop = true;    // 루프
                break;

            }

            m_ePreMotion = m_eCurMotion;
        }
    }
    else if (m_pFrameKey == L"GINGER_BRAVE_COOKIE_BLINK")
    {
        if (m_ePreMotion != m_eCurMotion)
        {
            switch (m_eCurMotion)
            {
            case RUN:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 7;
                m_tFrame.iMotion = 0;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 100;
                break;

            case JUMP:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 3;
                m_tFrame.iMotion = 1;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 100;
                break;


            case DOUBLE_JUMP:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 9;
                m_tFrame.iMotion = 2;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 50;
                break;


            case FALLING:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 1;
                m_tFrame.iMotion = 3;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 50;
                break;


            case LANDING:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 1;
                m_tFrame.iMotion = 4;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 50;
                break;


            case SLIDE:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 3;
                m_tFrame.iMotion = 5;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 50;
                break;


            case HIT:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 4;
                m_tFrame.iMotion = 6;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 100;
                break;


            case BONUS_TIME_INTRO:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 4;
                m_tFrame.iMotion = 7;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 100;
                break;

            case BONUS_TIME_UP:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 2;
                m_tFrame.iMotion = 8;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 100;
                break;

            case BONUS_TIME_DOWN:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 1;
                m_tFrame.iMotion = 9;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 100;
                break;

            case BONUS_TIME_OUTRO:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 2;
                m_tFrame.iMotion = 10;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 100;
                break;

            case BOOST:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 3;
                m_tFrame.iMotion = 11;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 200;
                break;

            case CLEAR:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 10;
                m_tFrame.iMotion = 12;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 200;
                break;

            case EXHAUST:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 3;
                m_tFrame.iMotion = 13;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 100;
                break;

            case DEAD:
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 4;
                m_tFrame.iMotion = 14;
                m_tFrame.dwTime = GetTickCount64();
                m_tFrame.dwSpeed = 100;
                break;

            }

            m_ePreMotion = m_eCurMotion;
        }
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

            m_eCurMotion = (m_iJumpCount == 0) ? JUMP : DOUBLE_JUMP;
            

            ++m_iJumpCount;
        }
        m_bWantJump = false;
        return;
    }

    if (!m_bOnGround)
    {
        if (m_fVy > eps)
            m_eCurMotion = FALLING;

        return;
    }

    if (bLanded)
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