#include "pch.h"
#include "CMenu.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"

CMenu::CMenu()
{
}

CMenu::~CMenu()
{
	Release();
}

void CMenu::Initialize()
{
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Start.bmp", L"Start");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Edit.bmp", L"Edit");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Exit.bmp", L"Exit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/Menu.bmp", L"Menu");

	//CObj* pButton = CAbstractFactory<CButton>::Create_Obj(200.f, 400.f);
	//pButton->Set_FrameKey(L"Start");
	//CObjMgr::Get_Instance()->Add_Object(BUTTON, pButton);

	//pButton = CAbstractFactory<CButton>::Create_Obj(400.f, 400.f);
	//pButton->Set_FrameKey(L"Edit");
	//CObjMgr::Get_Instance()->Add_Object(BUTTON, pButton);

	//pButton = CAbstractFactory<CButton>::Create_Obj(600.f, 400.f);
	//pButton->Set_FrameKey(L"Exit");
	//CObjMgr::Get_Instance()->Add_Object(BUTTON, pButton);

}

void CMenu::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CMenu::Render(HDC hDC)
{
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);

}

void CMenu::Release()
{
	//CObjMgr::Get_Instance()->Delete_ID(BUTTON);
}
