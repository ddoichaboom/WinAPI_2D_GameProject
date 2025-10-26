#include "pch.h"
#include "CButton.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"

const float ButtonfCX = 236.f;
const float ButtonfCY = 178.f;


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

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Jump_button.bmp", L"JUMP_BUITTON");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Slide_button.bmp", L"SLIDE_BUITTON");

	switch (m_eButtonType)
	{
	case JUMP:
		m_tInfo = { ButtonfCX / 2.f + 50.f, WINCY - (ButtonfCY / 2.f + 50.f)  , ButtonfCX, ButtonfCY };
		m_iDrawID = 0;
		m_pFrameKey = L"JUMP_BUITTON";
		break;

	case SLIDE:

		break;
	}
	 
	m_eRender = UI;

}

int CButton::Update(float deltaTime)
{
	Update_Rect();

	Key_Input();

    return OBJ_NOEVENT;
}

void CButton::Late_Update(float deltaTime)
{

}

void CButton::Render(HDC hDC)
{
	int 		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int 		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top ,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_iDrawID * (int)m_tInfo.fCX,
		0,
		(int)m_tInfo.fCX,   // 복사할 이미지 가로
		(int)m_tInfo.fCY,   // 복사할 이미지 세로
		RGB(0, 255, 0));    // 제거할 픽셀 색상 값
}

void CButton::Release()
{

}

void CButton::Key_Input()
{
	
	m_iDrawID = (CKeyMgr::Get_Instance()->Key_Pressing('J')) ? 1 : 0;

	//CKeyMgr::Get_Instance()->Key_Pressing('S');
}
