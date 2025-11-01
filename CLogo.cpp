#include "pch.h"
#include "CLogo.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");
}

void CLogo::Update(float fDeltaTime)
{
}

void CLogo::Late_Update(float fDeltaTime)
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_LOBBY);
		return;
	}

}

void CLogo::Render(HDC hDC)
{
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
}
