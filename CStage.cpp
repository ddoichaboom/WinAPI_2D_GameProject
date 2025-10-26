#include "pch.h"
#include "CStage.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include <ctime>
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CLineMgr.h"
#include "CButton.h"


CStage::CStage()
	: m_currTime(GetTickCount64()), m_prevTime(m_currTime), m_deltaTime(0)
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	//CLineMgr::Get_Instance()->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage01.bmp", L"STAGE01");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Slide_button.bmp", L"SLIDE_BUITTON");



	// 플레이어 생성
	CObjMgr::Get_Instance()->Add_Object(PLAYER, CAbstractFactory<CPlayer>::Create_Obj());

	// UI 버튼 생성
	CObjMgr::Get_Instance()->Add_Object(BUTTON, CAbstractFactory<CButton>::Create_Obj(JUMP));
	CObjMgr::Get_Instance()->Add_Object(BUTTON, CAbstractFactory<CButton>::Create_Obj(SLIDE));


	//CLineMgr::Get_Instance()->Initialize();
}

void CStage::Update()
{
	m_currTime = GetTickCount64();
	m_deltaTime = (float)(m_currTime - m_prevTime) / 1000.0;

	CObjMgr::Get_Instance()->Update(m_deltaTime);

	//CLineMgr::Get_Instance()->Update();


}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update(m_deltaTime);

	//CLineMgr::Get_Instance()->Late_Update();

	CScrollMgr::Get_Instance()->Scroll_Lock();

	m_prevTime = m_currTime;


}

void CStage::Render(HDC hDC)
{

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	//CLineMgr::Get_Instance()->Render(hDC);


	HDC     hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE01");

	BitBlt(hDC, iScrollX, iScrollY, 6393, 720, hGroundDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);

}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(PLAYER);
}
