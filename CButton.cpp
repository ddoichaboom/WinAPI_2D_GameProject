#include "pch.h"
#include "CButton.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CDataMgr.h"


CButton::CButton()
	: m_iDrawID(0)
{
}

CButton::~CButton()
{
	Release();
}

void CButton::Initialize()
{

	const IMAGEDATA* pJumpData = CDataMgr::Get_Instance()->Get_ImageData(L"JUMP_BUTTON");
	const IMAGEDATA* pSlideData = CDataMgr::Get_Instance()->Get_ImageData(L"SLIDE_BUTTON");


	switch (m_eButtonType)
	{
	case JUMP:
		m_tInfo = pJumpData->tInfo;
		m_tInfo.fX = (m_tInfo.fCX / 2.f) + 50.f;
		m_tInfo.fY = WINCY - (m_tInfo.fCX / 2.f);
		m_pFrameKey = pJumpData->pFrameKey;
		break;

	case SLIDE:
		m_tInfo = pSlideData->tInfo;
		m_tInfo.fX = WINCX - (m_tInfo.fCX / 2.f) - 50.f;
		m_tInfo.fY = WINCY - (m_tInfo.fCX / 2.f);
		m_pFrameKey = pSlideData->pFrameKey;		
		break;
	}

	m_iDrawID = 0;
}

int CButton::Update(float deltaTime)
{
	Update_Rect(UI_OBJECT);

	Key_Input();

    return OBJ_NOEVENT;
}

void CButton::Late_Update(float deltaTime)
{

}

void CButton::Render(HDC hDC)
{
	// 버튼 스프라이트 시트의 한 프레임 영역 계산
	int srcX = m_iDrawID * (int)m_tInfo.fCX;
	int srcY = 0;
	int srcW = (int)m_tInfo.fCX;
	int srcH = (int)m_tInfo.fCY;

	HDC     hBtnDC = CBmpMgr::Get_Instance()->Find_Image(Get_FrameKey());

	if (!hBtnDC) return;


	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	// UI는 화면 좌표 고정(스크롤 더하지 않음)
	int dstX = (int)(m_tRect.left);
	int dstY = (int)(m_tRect.top);
	int dstW = (int)(m_tInfo.fCX);
	int dstH = (int)(m_tInfo.fCY);

	// 소스 위치를 옮기려면 (시트 내 프레임 선택) -> Source DC를 프레임 시작점으로 오프셋한 보조 DC를 쓰거나
	// AlphaBlend의 Sx,Sy에 srcX,srcY를 지정하면 됨
	AlphaBlend(hDC,
		dstX, dstY,
		dstW, dstH,
		hBtnDC,
		srcX, srcY,
		srcW, srcH,
		bf);
}

void CButton::Release()
{

}

void CButton::Key_Input()
{
	
	switch (m_eButtonType)
	{
	case JUMP:
		m_iDrawID = (CKeyMgr::Get_Instance()->Key_Pressing('J')) ? 1 : 0;

		break;

	case SLIDE:
		m_iDrawID = (CKeyMgr::Get_Instance()->Key_Pressing('S')) ? 1 : 0;

		break;

	}

}
