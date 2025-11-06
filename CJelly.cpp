#include "pch.h"
#include "CJelly.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CDataMgr.h"
#include "CEffectMgr.h"
#include "Factory.h"

CJelly::CJelly()
	: m_cAlphabetChar(L'\0')
{
	ZeroMemory(&m_tJellyInfo, sizeof(JELLYINFO));
}

CJelly::~CJelly()
{
	Release();
}

void CJelly::Initialize()
{
	Set_Frame();
	
	if (wcsstr(m_pFrameKey.c_str(), L"ALPHABET_"))
	{
		// "JELLY_ALPHABET_A" 형태에서 마지막 문자 추출 
		size_t len = m_pFrameKey.length();
		if (len > 0)
			m_cAlphabetChar = m_pFrameKey[len - 1];
	}

	m_fSpeed = 0.f;				// 기본은 움직이지 않는 젤리
	m_fAngle = 0.f;
	m_eRender = GAMEOBJECT;

	m_PrevFrameKey = m_pFrameKey;
}

int CJelly::Update(float deltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;


	Update_Rect(JELLY);

	m_pTarget = CObjMgr::Get_Instance()->Get_Target(PLAYER, this);

	if (m_pTarget)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pTarget);

		if (pPlayer && pPlayer->Is_Change_Jelly_Mode())
		{
			if (wcsstr(m_pFrameKey.c_str(), L"JELLYBEAN"))
			{
				int value = rand() % 2;
				const wchar_t* newJellyKey = (value == 0) 
					? L"JELLY_BEAR_PINK" 
					: L"JELLY_BEAR_YELLOW";

				m_pFrameKey = newJellyKey;

				const IMAGEDATA* pJellyInfo = CDataMgr::Get_Instance()->Get_ImageData(m_pFrameKey);

				if (pJellyInfo)
				{
					m_tInfo.fCX = pJellyInfo->tInfo.fCX;
					m_tInfo.fCY = pJellyInfo->tInfo.fCY;
					m_tInfo.fHitCX = pJellyInfo->tInfo.fHitCX;
					m_tInfo.fHitCY = pJellyInfo->tInfo.fHitCY;
					m_tJellyInfo.iScore = 3000;
					Update_Rect(JELLY);

					CEffectMgr::Get_Instance()->Add_Effect(static_cast<CEffect*>(Create_Effect(m_tInfo.fHitX, m_tInfo.fHitY, L"EFFECT_ITEM_CHANGEJELLY")));
				}
			}
		}

		if (!pPlayer || !pPlayer->Is_Magnet_On())
		{
			m_pTarget = nullptr;
			m_fSpeed = 0.f;
			Update_Rect(JELLY);

			return OBJ_NOEVENT;
		}

		float fWidth = m_pTarget->Get_Info()->fHitX - m_tInfo.fHitX;
		float fHeight = m_pTarget->Get_Info()->fHitY - m_tInfo.fHitY;
		float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);		

		if (fDiagonal > pPlayer->Get_Magnet_Radius())
		{
			m_pTarget = nullptr;
			m_fSpeed = 0.f;
			Update_Rect(JELLY);

			return OBJ_NOEVENT;
		}

		m_fSpeed = 350.f;			

		float fRadian = acosf(fWidth / fDiagonal);
		m_fAngle = fRadian * 180.f / PI;
		if (m_tInfo.fHitY < m_pTarget->Get_Info()->fHitY)
			m_fAngle *= -1.f;

	}

	Update_Rect(JELLY);

	return OBJ_NOEVENT;
}

void CJelly::Late_Update(float deltaTime)
{
	if (m_PrevFrameKey != m_pFrameKey)
	{
		Set_Frame();

		m_PrevFrameKey = m_pFrameKey;
	}

	Move_Frame(deltaTime);

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f)) * deltaTime;
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f)) * deltaTime;

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

void CJelly::Set_Frame()
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
				m_tFrame.frameIntervalSec = 0.5f;
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
				m_tFrame.frameIntervalSec = 0.50f;
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
				m_tFrame.frameIntervalSec = 0.5f;
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
				m_tFrame.frameIntervalSec = 0.5f;
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
}

