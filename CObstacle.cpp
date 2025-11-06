#include "pch.h"
#include "CObstacle.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CDataMgr.h"
#include "CAbstractFactory.h"
#include "CJelly.h"
#include "CEffectMgr.h"
#include "Factory.h"

CObstacle::CObstacle()
	:m_bConverted(false)
{

}

CObstacle::~CObstacle()
{
    Release();
}

void CObstacle::Initialize()
{
	Set_Frame();

    m_eRender = GAMEOBJECT;
	m_bConverted = false;


}

int CObstacle::Update(float deltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect(OBSTACLE);

	m_pTarget = CObjMgr::Get_Instance()->Get_Target(PLAYER);

	if (m_pTarget && !m_bConverted)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pTarget);

		if (pPlayer && pPlayer->Is_Change_Obstacle_To_Coin_Mode())
		{
			const IMAGEDATA* pCoinInfo = CDataMgr::Get_Instance()->Get_ImageData(L"JELLY_COIN1");

			if (pCoinInfo)
			{
				float coinWidth = pCoinInfo->tInfo.fHitCX;
				float coinHeight = pCoinInfo->tInfo.fHitCY;

				int cols = (int)(m_tInfo.fHitCX / coinWidth);
				int rows = (int)(m_tInfo.fHitCY / coinHeight);

				float startX = m_tInfo.fHitX - (m_tInfo.fHitCX * 0.5f);
				float startY = m_tInfo.fHitY - (m_tInfo.fHitCY * 0.5f);

				for (int row = 0; row < rows; ++row)
				{
					for (int col = 0; col < cols; ++col)
					{
						float coinX = startX + (col * coinWidth) + (coinWidth * 0.5f);
						float coinY = startY + (row * coinHeight) + (coinHeight * 0.5f);

						CObj* pCoin = CAbstractFactory<CJelly>::Create_Obj(coinX, coinY, 
							CDataMgr::Get_Instance()->Get_ImageData(L"JELLY_COIN1"));
						CObjMgr::Get_Instance()->Add_Object(JELLY, pCoin);

						CEffectMgr::Get_Instance()->Add_Effect(static_cast<CEffect*>(Create_Effect(coinX, coinY, L"EFFECT_ITEM_COIN")));
					}
				}

				m_bConverted = true;
				m_bDead = true;

			}
		}
	}

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

void        CObstacle::Set_Frame()
{
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
			m_tFrame.bLoop = false;    // 루프
		}
		else if (wcsstr(m_pFrameKey.c_str(), L"OBSTACLE02") || wcsstr(m_pFrameKey.c_str(), L"OBSTACLE03"))
		{
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.frameElapsedSec = 0.0f;
			m_tFrame.frameIntervalSec = 0.2f;
			m_tFrame.stateLockRemainSec = 0.0f;
			m_tFrame.bLoop = true;    // 루프
		}
		else if (wcsstr(m_pFrameKey.c_str(), L"OBSTACLE04") || wcsstr(m_pFrameKey.c_str(), L"OBSTACLE05"))
		{
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.frameElapsedSec = 0.0f;
			m_tFrame.frameIntervalSec = 0.00f;
			m_tFrame.stateLockRemainSec = 0.0f;
			m_tFrame.bLoop = false;    // 루프
		}
	}
	else 
	{
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 0;
		m_tFrame.iMotion = 0;
		m_tFrame.frameElapsedSec = 0.0f;
		m_tFrame.frameIntervalSec = 0.00f;
		m_tFrame.stateLockRemainSec = 0.0f;
		m_tFrame.bLoop = false;    // 루프
	}


}

