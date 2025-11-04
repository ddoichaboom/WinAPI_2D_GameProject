#include "pch.h"
#include "CBonusTime.h"
#include "CDataMgr.h"
#include "CBmpMgr.h"

CBonusTime::CBonusTime()
	:m_pBgFrameKey(L"UI_BONUSTIME_BG"),
	m_pEffectFrameKey(L"UI_BONUSTIME_EFFECT"),
	m_pBgInfo(nullptr), m_pEffectInfo(nullptr),
	m_fEffectPosX(0.f), m_fEffectSpeed(200.f)
{

}

CBonusTime::~CBonusTime()
{

}

void CBonusTime::Initialize()
{
	m_pBgInfo = CDataMgr::Get_Instance()->Get_ImageData(m_pBgFrameKey);
	//m_pEffectInfo = CDataMgr::Get_Instance()->Get_ImageData(m_pEffectFrameKey);

	m_tInfo = m_pBgInfo->tInfo;

	m_tInfo.fX = 10.f;
	m_tInfo.fY = 50.f;

	m_eRender = UI;
}

int CBonusTime::Update(float deltaTime)
{
	return OBJ_NOEVENT;
}

void CBonusTime::Late_Update(float deltaTime)
{
}

void CBonusTime::Render(HDC hDC)
{
	HDC hBgDC = CBmpMgr::Get_Instance()->Find_Image(m_pBgFrameKey);

	if (!hBgDC)
		return;

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	float fRenderX = m_tInfo.fX - (m_tInfo.fCX / 2.f);
	float fRenderY = m_tInfo.fY - (m_tInfo.fCY / 2.f);

	AlphaBlend(hDC,
		(int)m_tInfo.fX, (int)m_tInfo.fY,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hBgDC,
		0, 0,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,
		bf);
}

void CBonusTime::Release()
{

}