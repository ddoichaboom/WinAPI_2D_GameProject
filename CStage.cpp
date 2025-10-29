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
#include "CTile.h"
#include "CTileMgr.h"


CStage::CStage()
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

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Platform/Stage01/Platform01.png", L"STAGE01_PLATFORM01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Platform/Stage01/Platform02.png", L"STAGE01_PLATFORM02");


	// 플레이어 생성
	CObjMgr::Get_Instance()->Add_Object(PLAYER, CAbstractFactory<CPlayer>::Create_Obj());

	// UI 버튼 생성
	CObjMgr::Get_Instance()->Add_Object(BUTTON, CAbstractFactory<CButton>::Create_Obj(JUMP));
	CObjMgr::Get_Instance()->Add_Object(BUTTON, CAbstractFactory<CButton>::Create_Obj(SLIDE));

	CTileMgr::Get_Instance()->Load_Tile();
	auto& vecTile =  CTileMgr::Get_Instance()->Get_vecTile();

	for (auto& pTile : vecTile)
	{
		if (!pTile)
			continue;

		CObjMgr::Get_Instance()->Add_Object(
			PLATFORM,
			CAbstractFactory<CTile>::Create_Obj(
				pTile->Get_Info()->fX,
				pTile->Get_Info()->fY,
				pTile->Get_FrameKey()));
	}

	CScrollMgr::Get_Instance()->Set_ScrollX(0.f);
	CScrollMgr::Get_Instance()->Set_ScrollY(0.f);
}

void CStage::Update()
{
	m_currTime = GetTickCount64();
	m_deltaTime = (float)(m_currTime - m_prevTime) / 1000.0;

	CObjMgr::Get_Instance()->Update(m_deltaTime);

	CObjMgr::Get_Instance()->Check_Collision(PLAYER, PLATFORM, CObjMgr::RECT);

	m_prevTime = m_currTime;

}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update(m_deltaTime);


	CScrollMgr::Get_Instance()->Scroll_Lock();




}

void CStage::Render(HDC hDC)
{

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	//CLineMgr::Get_Instance()->Render(hDC);


	HDC     hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE01");

	BitBlt(hDC, iScrollX, iScrollY, STAGE01_SIZE, WINCY, hGroundDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);

	if (CObjMgr::Get_Instance()->Get_Target(PLAYER))
	{
		wchar_t szPlayer[64] = L"";
		swprintf_s(szPlayer, _countof(szPlayer),
			L"Player fY : %ld fVY : %ld   ",
			(long)CObjMgr::Get_Instance()->Get_Target(PLAYER)->Get_Info()->fY,
			(long)CObjMgr::Get_Instance()->Get_Target(PLAYER)->Get_Y_Axis_Speed());

		RECT rcPlayerPos = { 10, 100, 100, 120 };
		DrawTextW(hDC, szPlayer, -1, &rcPlayerPos, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOCLIP);
	}
}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(PLAYER);
}
