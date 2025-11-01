#include "pch.h"
#include "CMainGame.h"
#include "CAbstractFactory.h"
#include "CSceneMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CTileMgr.h"
#include "CSoundMgr.h"
#include <tchar.h>
#include "CTimeMgr.h"
#include "CDataMgr.h"
#include "CUiMgr.h"

CMainGame::CMainGame()
	: m_dwTime(GetTickCount64()), m_iFPS(0)
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

	CDataMgr::Get_Instance()->Initialize();

	CTimeMgr::Get_Instance()->Initialize();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"./Image/Back.bmp", L"Back");
	CSoundMgr::Get_Instance()->Initialize();

	CSceneMgr::Get_Instance()->Scene_Change(SC_LOBBY);


#ifdef _DEBUG

	TCHAR szPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szPath);
	OutputDebugString(L"현재 작업 디렉토리: ");
	OutputDebugString(szPath);
	OutputDebugString(L"\n");

#endif // _DEBUG


}

void CMainGame::Update()
{
	CTimeMgr::Get_Instance()->Update_DeltaTime();

	const float fixedDt = CTimeMgr::Get_Instance()->m_fFixedDeltaTime;
	int iSteps = 0;
	const int MAX_STEPS = 5;


	while (CTimeMgr::Get_Instance()->m_fAccumulator >= fixedDt)
		{
			float fDeltaTime = CTimeMgr::Get_Instance()->Get_DeltaTime();

			CSceneMgr::Get_Instance()->Update(fDeltaTime);
			CSceneMgr::Get_Instance()->Late_Update(fDeltaTime);
			CKeyMgr::Get_Instance()->Update();

			CTimeMgr::Get_Instance()->m_fAccumulator -= fDeltaTime;

			if (++iSteps >= MAX_STEPS)
			{
				// 남은 누적분은 버리거나(여기서는 버림) 보간용으로만 유지하는 선택도 가능
				CTimeMgr::Get_Instance()->m_fAccumulator = 0.f;
				break;
			}
		}
}

void CMainGame::Late_Update()
{

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

	CDataMgr::Get_Instance()->Destroy_Instance();
	CTimeMgr::Get_Instance()->Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CUiMgr::Get_Instance()->Destroy_Instance();
}
