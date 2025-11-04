#include "pch.h"
#include "CLobby.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CSoundMgr.h"

CLobby::CLobby()
{
}

CLobby::~CLobby()
{
	Release();
}

void CLobby::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"./Image/Lobby/Lobby.png", L"LOBBY");

	CSoundMgr::Get_Instance()->PlayBGM(L"./Sound/Main_Lobby_BGM.mp3", 0.5f);
}

void CLobby::Update(float fDeltaTime)
{
	Key_Input();
}

void CLobby::Late_Update(float fDeltaTime)
{

}

void CLobby::Render(HDC hDC)
{
	HDC     hLobbyDC = CBmpMgr::Get_Instance()->Find_Image(L"LOBBY");

	if (!hLobbyDC) 
		return;

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	// 소스 위치를 옮기려면 (시트 내 프레임 선택) -> Source DC를 프레임 시작점으로 오프셋한 보조 DC를 쓰거나
	// AlphaBlend의 Sx,Sy에 srcX,srcY를 지정하면 됨
	AlphaBlend(hDC,
		0, 0,
		WINCX, WINCY,
		hLobbyDC,
		0, 0,
		WINCX, WINCY,
		bf);

}

void CLobby::Release()
{
}

void CLobby::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE);
		return;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('E'))
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_EDIT);
		return;
	}
}