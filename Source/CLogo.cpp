#include "pch.h"
#include "CLogo.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CSoundMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{

	CSoundMgr::Get_Instance()->PlaySound(L"./Sound/Intro.mp3", SOUND_INTRO, 0.5f);
	 
}

void CLogo::Update(float fDeltaTime)
{
}

void CLogo::Late_Update(float fDeltaTime)
{
	if (!CSoundMgr::Get_Instance()->Is_Playing(SOUND_INTRO))
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_LOBBY);
		return;
	}

}

void CLogo::Render(HDC hDC)
{
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"LOGO");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
}
