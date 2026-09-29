#include "pch.h"
#include "CHpBar.h"
#include "CDataMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"

CHpBar::CHpBar()
	: m_pBgFrameKey(L"UI_HP_BAR_BG"),
	m_pGaugeFrameKey(L"UI_HP_BAR"),
	m_pBgInfo(nullptr), m_pGaugeInfo(nullptr),
	m_fRatio(1.f)
{
}

CHpBar::~CHpBar()
{
	Release();
}

void    CHpBar::Initialize()
{
	m_pBgInfo = CDataMgr::Get_Instance()->Get_ImageData(m_pBgFrameKey);
	m_pGaugeInfo = CDataMgr::Get_Instance()->Get_ImageData(m_pGaugeFrameKey);

	m_tInfo = m_pBgInfo->tInfo;

	m_tInfo.fX = WINCX / 2.f;
	m_tInfo.fY = 50.f;
}

int     CHpBar::Update(float fDeltaTime)
{
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Target(PLAYER);

	if (pPlayer)
	{
		const PLAYERINFO* pPlayerInfo = static_cast<CPlayer*>(pPlayer)->Get_PlayerInfo();
		if (pPlayerInfo->fMaxHp > 0.f)
		{
			m_fRatio = pPlayerInfo->fHp / pPlayerInfo->fMaxHp;
		}
		else
			m_fRatio = 0.f;
	}

	return 0;
}

void    CHpBar::Late_Update(float fDeltaTime)
{
}

void    CHpBar::Render(HDC hDC)
{
	HDC hBgDC = CBmpMgr::Get_Instance()->Find_Image(m_pBgFrameKey);
	HDC hGaugeDC = CBmpMgr::Get_Instance()->Find_Image(m_pGaugeFrameKey);

	if (!hBgDC || !hGaugeDC)
		return;

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	float	bgRenderX = m_tInfo.fX - (m_tInfo.fCX / 2.f);
	float	bgRenderY = m_tInfo.fY - (m_tInfo.fCY / 2.f);


	// 체력 바 백그라운드 이미지 출력 
	AlphaBlend(hDC,
		(int)bgRenderX, (int)bgRenderY,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hBgDC,
		0, 0,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,
		bf);

	float fGaugeOffsetX = 53.f;
	float fGaugeOffsetY = (m_pBgInfo->tInfo.fCY - m_pGaugeInfo->tInfo.fCY) / 2.f;

	int iRenderGaugthWidth = (int)(m_pGaugeInfo->tInfo.fCX * m_fRatio);

	AlphaBlend(hDC,
		(int)(bgRenderX + fGaugeOffsetX), (int)(bgRenderY + fGaugeOffsetY),
		iRenderGaugthWidth, (int)m_pGaugeInfo->tInfo.fCY,
		hGaugeDC,
		0, 0,
		iRenderGaugthWidth, (int)m_pGaugeInfo->tInfo.fCY,
		bf);

}

void    CHpBar::Release()
{

}
