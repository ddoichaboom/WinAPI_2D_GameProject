#include "pch.h"
#include "CJelly.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CJelly::CJelly()
{
	ZeroMemory(&m_tJellyInfo, sizeof(JELLYINFO));
}

CJelly::~CJelly()
{
	Release();
}

void CJelly::Initialize()
{
	if (wcsstr(m_pFrameKey.c_str(), L"JELLY"))
	{
		if (wcsstr(m_pFrameKey.c_str(), L"_JELLYBEAN"))
		{
			// 기본 젤리 세팅
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.frameElapsedSec = 0.0f;
			m_tFrame.frameIntervalSec = 0.10f;
			m_tFrame.stateLockRemainSec = 0.00f;
			m_tFrame.bLoop = true;    // 루프

			m_tJellyInfo.iScore = 1000;
		}
		else if (wcsstr(m_pFrameKey.c_str(), L"_BEAR"))
		{
			if (wcsstr(m_pFrameKey.c_str(), L"_PINK"))
			{
				// 기본 젤리 세팅
				m_tFrame.iStart = 0;
				m_tFrame.iEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.frameElapsedSec = 0.0f;
				m_tFrame.frameIntervalSec = 0.10f;
				m_tFrame.stateLockRemainSec = 0.00f;
				m_tFrame.bLoop = true;    // 루프

				m_tJellyInfo.iScore = 3000;

			}
			else if (wcsstr(m_pFrameKey.c_str(), L"_YELLOW"))
			{
				// 기본 젤리 세팅
				m_tFrame.iStart = 0;
				m_tFrame.iEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.frameElapsedSec = 0.0f;
				m_tFrame.frameIntervalSec = 0.10f;
				m_tFrame.stateLockRemainSec = 0.00f;
				m_tFrame.bLoop = true;    // 루프

				m_tJellyInfo.iScore = 3000;

			}
			else if (wcsstr(m_pFrameKey.c_str(), L"_BIG"))
			{
				m_tFrame.iStart = 0;
				m_tFrame.iEnd = 1;
				m_tFrame.iMotion = 0;
				m_tFrame.frameElapsedSec = 0.0f;
				m_tFrame.frameIntervalSec = 0.30f;
				m_tFrame.stateLockRemainSec = 0.0f;
				m_tFrame.bLoop = true;    // 루프

				m_tJellyInfo.iScore = 10000;

			}

		}
		else if (wcsstr(m_pFrameKey.c_str(), L"_COIN1"))
		{
			if (wcsstr(m_pFrameKey.c_str(), L"_BIG"))
			{
				m_tFrame.iStart = 0;
				m_tFrame.iEnd = 3;
				m_tFrame.iMotion = 0;
				m_tFrame.frameElapsedSec = 0.0f;
				m_tFrame.frameIntervalSec = 0.20f;
				m_tFrame.stateLockRemainSec = 0.0f;
				m_tFrame.bLoop = true;    // 루프

				m_tJellyInfo.iCoin = 100;
				m_tJellyInfo.iScore = 3000;
			}
			else if (wcsstr(m_pFrameKey.c_str(), L""))
			{
				m_tFrame.iStart = 0;
				m_tFrame.iEnd = 3;
				m_tFrame.iMotion = 0;
				m_tFrame.frameElapsedSec = 0.0f;
				m_tFrame.frameIntervalSec = 0.20f;
				m_tFrame.stateLockRemainSec = 0.0f;
				m_tFrame.bLoop = true;    // 루프

				m_tJellyInfo.iCoin = 1;
				m_tJellyInfo.iScore = 500;
			}
		}
		else if (wcsstr(m_pFrameKey.c_str(), L"_COIN2"))
		{
			if (wcsstr(m_pFrameKey.c_str(), L"_BIG"))
			{
				m_tFrame.iStart = 0;
				m_tFrame.iEnd = 3;
				m_tFrame.iMotion = 0;
				m_tFrame.frameElapsedSec = 0.0f;
				m_tFrame.frameIntervalSec = 0.12f;
				m_tFrame.stateLockRemainSec = 0.0f;
				m_tFrame.bLoop = true;    // 루프

				m_tJellyInfo.iCoin = 1000;
				m_tJellyInfo.iScore = 5000;
			}
			else if (wcsstr(m_pFrameKey.c_str(), L""))
			{
				m_tFrame.iStart = 0;
				m_tFrame.iEnd = 5;
				m_tFrame.iMotion = 0;
				m_tFrame.frameElapsedSec = 0.0f;
				m_tFrame.frameIntervalSec = 0.12f;
				m_tFrame.stateLockRemainSec = 0.0f;
				m_tFrame.bLoop = true;    // 루프

				m_tJellyInfo.iCoin = 50;
				m_tJellyInfo.iScore = 1000;
			}
		}
		else if (wcsstr(m_pFrameKey.c_str(), L"_ALPHABET"))
		{
			// 기본 젤리 세팅
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.frameElapsedSec = 0.0f;
			m_tFrame.frameIntervalSec = 0.10f;
			m_tFrame.stateLockRemainSec = 0.00f;
			m_tFrame.bLoop = true;    // 루프

			m_tJellyInfo.iScore = 500;
		}
		else if (wcsstr(m_pFrameKey.c_str(), L"_EXCLAMATION_MARK"))
		{
			// 기본 젤리 세팅
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.frameElapsedSec = 0.0f;
			m_tFrame.frameIntervalSec = 0.10f;
			m_tFrame.stateLockRemainSec = 0.00f;
			m_tFrame.bLoop = true;    // 루프

			m_tJellyInfo.iScore = 500;
			
		}
	}

	m_eRender = GAMEOBJECT;
}

int CJelly::Update(float deltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect(JELLY);

	return OBJ_NOEVENT;
}

void CJelly::Late_Update(float deltaTime)
{
	Move_Frame(deltaTime);

	Update_Rect(JELLY);

}

void CJelly::Render(HDC hDC)
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

	HDC hJellyDC = CBmpMgr::Get_Instance()->Find_Image(Get_FrameKey());

	if (!hJellyDC)
		return;



	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	

	AlphaBlend(hDC,
			dstX, dstY,
			dstW, dstH,
			hJellyDC,
			srcX, srcY,
			srcW, srcH,
			bf);

	
}

void CJelly::Release()
{
}

