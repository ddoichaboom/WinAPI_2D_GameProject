#include "pch.h"
#include "CEdit.h"
//#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"

CEdit::CEdit()
{
}

CEdit::~CEdit()
{
	Release();
}

void CEdit::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Tile.bmp", L"Tile");

	//CTileMgr::Get_Instance()->Initialize();
}

void CEdit::Update()
{
	//CTileMgr::Get_Instance()->Update();

	Key_Input();
}

void CEdit::Late_Update()
{
	//CTileMgr::Get_Instance()->Late_Update();
}

void CEdit::Render(HDC hDC)
{
	//CTileMgr::Get_Instance()->Render(hDC);
}

void CEdit::Release()
{
}

void CEdit::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		//CTileMgr::Get_Instance()->Picking(pt, 1, 0);
	}

	//if (CKeyMgr::Get_Instance()->Key_Down('S'))
		//CTileMgr::Get_Instance()->Save_Tile();

	//if (CKeyMgr::Get_Instance()->Key_Down('L'))
		//CTileMgr::Get_Instance()->Load_Tile();

}

