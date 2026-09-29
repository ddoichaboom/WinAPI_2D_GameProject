#include "pch.h"
#include "CBossStage.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include <ctime>
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"


CBossStage::CBossStage()
	: m_currTime(GetTickCount64()), m_prevTime(m_currTime), m_deltaTime(0)
{
}

CBossStage::~CBossStage()
{
	Release();
}

void CBossStage::Initialize()
{
	// 플레이어 생성
	//CObjMgr::Get_Instance()->Add_Object(PLAYER, CAbstractFactory<CPlayer>::Create_Obj());


	//CLineMgr::Get_Instance()->Initialize();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
}

void CBossStage::Update()
{
	m_currTime = GetTickCount64();
	m_deltaTime = (float)(m_currTime - m_prevTime) / 1000.0;

	CObjMgr::Get_Instance()->Update(m_deltaTime);
}

void CBossStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update(m_deltaTime);

	CScrollMgr::Get_Instance()->Scroll_Lock();

	m_prevTime = m_currTime;


}

void CBossStage::Render(HDC hDC)
{

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC     hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");

	BitBlt(hDC, iScrollX, iScrollY, 1920, 1280, hGroundDC, 0, 0, SRCCOPY);


	CObjMgr::Get_Instance()->Render(hDC);
}

void CBossStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(PLAYER);
}
