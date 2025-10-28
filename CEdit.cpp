#include "pch.h"
#include "CEdit.h"
//#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"

CEdit::CEdit()
	: m_bTestMode(false), m_bEditMode(false), m_iSequence(1),
	m_bRenderVerticalLine(false)
{
	ZeroMemory(&m_tCursor, sizeof(POINT));
}

CEdit::~CEdit()
{
	Release();
}

void CEdit::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage01.bmp", L"STAGE01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage02.bmp", L"STAGE02");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Platform/Stage01/Platform01.png", L"STAGE01_PLATFORM01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Platform/Stage01/Platform02.png", L"STAGE01_PLATFORM02");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Platform/Stage02/Platform01.png", L"STAGE02_PLATFORM01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Platform/Stage02/Platform02.png", L"STAGE02_PLATFORM02");


	CTileMgr::Get_Instance()->Initialize();
}

void CEdit::Update()
{
	
	m_currTime = GetTickCount64();
	m_deltaTime = (float)(m_currTime - m_prevTime) / 1000.0;

	GetCursorPos(&m_tCursor);
	ScreenToClient(g_hWnd, &m_tCursor);

	m_tCursor.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	m_tCursor.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	Key_Input();

	CTileMgr::Get_Instance()->Update();

	if (m_bTestMode)
		CObjMgr::Get_Instance()->Update(m_deltaTime);

	

	
}

void CEdit::Late_Update()
{
	if (m_bTestMode)
		CObjMgr::Get_Instance()->Late_Update(m_deltaTime);

		
	CTileMgr::Get_Instance()->Late_Update();

	 
	CScrollMgr::Get_Instance()->Scroll_Lock();

	m_prevTime = m_currTime;



}

void CEdit::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	//CLineMgr::Get_Instance()->Render(hDC);

	HDC     hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE01");

	BitBlt(hDC, iScrollX, iScrollY, STAGE01_SIZE, WINCY, hGroundDC, 0, 0, SRCCOPY);

	CTileMgr::Get_Instance()->Render(hDC);


	// 수직선 생성
	if (m_bRenderVerticalLine)
	{
		for (int i = 0; i < (int)STAGE01_SIZE / TILECX ; i++)
		{
			MoveToEx(hDC, i * TILECX + iScrollX, 0, nullptr);
			LineTo(hDC, i * TILECX + iScrollX, WINCY);
		}
	}

	// 마우스 위치에 두번째 플랫폼이 따라다니며 렌더링 되어야 함
	if (m_bEditMode && (m_iSequence == 2))
	{
		Rectangle(hDC,
			m_tCursor.x + iScrollX - 62,
			m_tCursor.y + iScrollY  - 39 / 2.f,
			m_tCursor.x + iScrollX + 62,
			m_tCursor.y + iScrollY + 39 / 2.f);
	}


	if (m_bTestMode)
		CObjMgr::Get_Instance()->Render(hDC);

	// 모드 & 마우스 좌표 출력
	Edit_State(hDC);
}

void CEdit::Release()
{

}

void CEdit::Key_Input()
{
	POINT pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (CKeyMgr::Get_Instance()->Key_Down('E'))
		m_bEditMode = !m_bEditMode;

	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		CScrollMgr::Get_Instance()->Set_ScrollX(10.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		CScrollMgr::Get_Instance()->Set_ScrollX(-10.f);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (m_bEditMode)
		{
			switch (m_iSequence)
			{
			case 1:
				CTileMgr::Get_Instance()->Picking(pt, true);

				break;

			case 2:
				CTileMgr::Get_Instance()->Add_Tile(pt.x, pt.y);
				break;
			}
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		if (m_bEditMode)
		{
			switch (m_iSequence)
			{
			case 1:
				CTileMgr::Get_Instance()->Picking(pt, false);
				break;

			case 2:
				CTileMgr::Get_Instance()->Picking(pt, false);
				break;
			}
		} 
		
	}

	if (CKeyMgr::Get_Instance()->Key_Down('P'))
	{
		m_bTestMode = !m_bTestMode;

		if (m_bTestMode)
		{
			if (!CObjMgr::Get_Instance()->Get_Target(PLAYER, nullptr))
			{
				CObjMgr::Get_Instance()->Add_Object(PLAYER, CAbstractFactory<CPlayer>::Create_Obj());
			}
		}
		else
		{
			CObjMgr::Get_Instance()->Delete_ID(PLAYER);
		}
		
	}

	if (CKeyMgr::Get_Instance()->Key_Down('M'))
		CTileMgr::Get_Instance()->Save_Tile();

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
		CTileMgr::Get_Instance()->Load_Tile();

	if (CKeyMgr::Get_Instance()->Key_Down('O'))
	{
		if (m_iSequence < 4)
			m_iSequence++;
		else
			m_iSequence = 1;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('R'))
	{
		m_bRenderVerticalLine = !m_bRenderVerticalLine;
	}

}

void CEdit::Edit_State(HDC hDC)
{

	int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();


	// 화면 고정 텍스트 옵션
// POINT screenPt = m_tCursor; // 월드좌표 → 화면좌표
	POINT screenPt;
	screenPt.x = (LONG)(m_tCursor.x + iScrollX);
	screenPt.y = (LONG)(m_tCursor.y + iScrollY);

	int oldBk = SetBkMode(hDC, TRANSPARENT);
	COLORREF oldColor = SetTextColor(hDC, RGB(255, 255, 0));

	wchar_t szCursor[64] = L"";
	swprintf_s(szCursor, _countof(szCursor),
		L"ptMouse.x: %ld   ptMouse.y: %ld",
		(long)m_tCursor.x, (long)m_tCursor.y);

	// 커서 근처에 라벨로 표시
	RECT rc = { 10, 50, 300, 80 };
	DrawTextW(hDC, szCursor, -1, &rc, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOCLIP);

	RECT rcState = { 10, 10, 100, 30 };
	wchar_t szEditState[256] = L"";
	const wchar_t* EditMode = m_bEditMode ? L"ON" : L"OFF";
	const wchar_t* TestMode = m_bTestMode ? L"ON" : L"OFF";
	const wchar_t* VerticalLineMode = m_bRenderVerticalLine ? L"ON" : L"OFF";


	swprintf_s(szEditState, _countof(szEditState),
		L"편집 모드 (E) : %s  테스트 모드 (P) : %s  수직선 (R) : %s  현재 단계 (O) : %d",
		EditMode, TestMode, VerticalLineMode, m_iSequence);
	DrawTextW(hDC, szEditState, -1, &rcState, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOCLIP);


	// (선택) 십자선 마커
	MoveToEx(hDC, screenPt.x - 5, screenPt.y, nullptr);
	LineTo(hDC, screenPt.x + 5, screenPt.y);
	MoveToEx(hDC, screenPt.x, screenPt.y - 5, nullptr);
	LineTo(hDC, screenPt.x, screenPt.y + 5);

	SetTextColor(hDC, oldColor);
	SetBkMode(hDC, oldBk);
}
