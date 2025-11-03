#include "pch.h"
#include "CObstacle.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CObstacle::CObstacle()
{

}

CObstacle::~CObstacle()
{
    Release();
}

void CObstacle::Initialize()
{
    m_eRender = GAMEOBJECT;

	if (wcsstr(m_pFrameKey.c_str(), L"STAGE02"))
	{
		if (wcsstr(m_pFrameKey.c_str(), L"OBSTACLE01"))
		{
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 1;
			m_tFrame.iMotion = 0;
			m_tFrame.frameElapsedSec = 0.0f;
			m_tFrame.frameIntervalSec = 0.50f;
			m_tFrame.stateLockRemainSec = 0.0f;
			m_tFrame.bLoop = false;    // 风橇
		}
		else if (wcsstr(m_pFrameKey.c_str(), L"OBSTACLE02")|| wcsstr(m_pFrameKey.c_str(), L"OBSTACLE03"))
		{
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.frameElapsedSec = 0.0f;
			m_tFrame.frameIntervalSec = 0.2f;
			m_tFrame.stateLockRemainSec = 0.0f;
			m_tFrame.bLoop = true;    // 风橇
		}
		else if (wcsstr(m_pFrameKey.c_str(), L"OBSTACLE04") || wcsstr(m_pFrameKey.c_str(), L"OBSTACLE05"))
		{
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.frameElapsedSec = 0.0f;
			m_tFrame.frameIntervalSec = 0.00f;
			m_tFrame.stateLockRemainSec = 0.0f;
			m_tFrame.bLoop = false;    // 风橇
		}
	}

	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.frameElapsedSec = 0.0f;
	m_tFrame.frameIntervalSec = 0.00f;
	m_tFrame.stateLockRemainSec = 0.0f;
	m_tFrame.bLoop = false;    // 风橇
}

int CObstacle::Update(float deltaTime)
{
	Update_Rect(OBSTACLE);

    return OBJ_NOEVENT;
}

void CObstacle::Late_Update(float deltaTime)
{
	Move_Frame(deltaTime);

	Update_Rect(OBSTACLE);
}

void CObstacle::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int srcX = (int)m_tInfo.fCX * m_tFrame.iStart;
	int srcY = (int)m_tInfo.fCY * m_tFrame.iMotion;
	int srcW = (int)m_tInfo.fCX;
	int srcH = (int)m_tInfo.fCY;

	int dstX = (int)m_tRect.left + iScrollX;
	int dstY = (int)m_tRect.top + iScrollY;
	int dstW = (int)m_tInfo.fCX;
	int dstH = (int)m_tInfo.fCY;

	HDC hObstacleDC = CBmpMgr::Get_Instance()->Find_Image(Get_FrameKey());

	if (!hObstacleDC)
		return;

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };


	AlphaBlend(hDC,
		dstX, dstY,
		dstW, dstH,
		hObstacleDC,
		srcX, srcY,
		srcW, srcH,
		bf);
}

void CObstacle::Release()
{
}


