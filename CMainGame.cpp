#include "pch.h"
#include "CMainGame.h"
#include "CAbstractFactory.h"
#include "CSceneMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CLineMgr.h"

CMainGame::CMainGame()
	: m_dwTime(GetTickCount64()), m_iFPS(0),
	m_currTime(GetTickCount64()),m_prevTime(m_currTime), m_deltaTime(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	srand(unsigned(time(NULL)));

	m_hDC = GetDC(g_hWnd);

	CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE01);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");




}

void CMainGame::Update()
{

	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Update();

}

void CMainGame::Render()
{
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount64();
	}

	HDC     hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");

	CSceneMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	//CTileMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CLineMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
}
