#include "pch.h"
#include "CLineMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
	: m_bLineStart(false), m_bEditorMode(false)
{
	ZeroMemory(m_tLinePoint, sizeof(m_tLinePoint));
	ZeroMemory(&m_tCursor, sizeof(POINT));
}

CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	//Load_Line();

}

int CLineMgr::Update()
{
	GetCursorPos(&m_tCursor);
	ScreenToClient(g_hWnd, &m_tCursor);

	m_tCursor.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	m_tCursor.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	/*if (CKeyMgr::Get_Instance()->Key_Down('M'))
	{
		m_bEditorMode = !m_bEditorMode;

		if (m_bEditorMode)
			MessageBox(g_hWnd, L"에디터 모드 ON", _T("On"), MB_OK);
		else
			MessageBox(g_hWnd, L"에디터 모드 OFF", _T("Off"), MB_OK);

		m_tLinePoint[LEFT].fX = 0;
		m_tLinePoint[LEFT].fY = 0;
		m_tLinePoint[RIGHT].fX = 0;
		m_tLinePoint[RIGHT].fY = 0;
		m_bLineStart = true;

	}*/

	if (m_bEditorMode)
	{ 
		if (GetAsyncKeyState('L'))
		{
			Load_Line();
			return 0;
		}

		if (GetAsyncKeyState('S'))
		{
			Save_Line();
			return 0;
		}
	}


	return 0;
}

void CLineMgr::Late_Update()
{
}

void CLineMgr::Render(HDC hDC)
{
	for (auto& pLine : m_LineList)
		pLine->Render(hDC);

	if (m_bEditorMode)
	{
		int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

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
		RECT rc = { screenPt.x + 12, screenPt.y - 16, screenPt.x + 312, screenPt.y + 16 };
		DrawTextW(hDC, szCursor, -1, &rc, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOCLIP);

		// (선택) 십자선 마커
		MoveToEx(hDC, screenPt.x - 5, screenPt.y, nullptr);
		LineTo(hDC, screenPt.x + 5, screenPt.y);
		MoveToEx(hDC, screenPt.x, screenPt.y - 5, nullptr);
		LineTo(hDC, screenPt.x, screenPt.y + 5);

		SetTextColor(hDC, oldColor);
		SetBkMode(hDC, oldBk);
	}

}

void CLineMgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), Safe_Delete<CLine*>);
	m_LineList.clear();
}

bool CLineMgr::Collision_Line(float* pY, float fX)
{
	if (m_LineList.empty())
		return false;


	CLine* pTargetLine = nullptr;

	for (auto& pLine : m_LineList)
	{
		if (fX >= pLine->Get_Info().tLeft.fX &&
			fX <= pLine->Get_Info().tRight.fX)
		{
			pTargetLine = pLine;
		}
	}

	if (!pTargetLine)
		return false;

	// Y - y1 = ((y2 - y1) / (x2 - x1)) * X - x1

	float x1 = pTargetLine->Get_Info().tLeft.fX;
	float y1 = pTargetLine->Get_Info().tLeft.fY;

	float x2 = pTargetLine->Get_Info().tRight.fX;
	float y2 = pTargetLine->Get_Info().tRight.fY;


	*pY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
}

CLine* CLineMgr::Line_Movement_Circle(INFO* m_pInfo)
{
	CLine* pTargetLine = nullptr;

	float  fDistance(0.f);
	float  fTarget(INFINITY);


	if (m_LineList.empty())
		return nullptr;

	for (auto& pLine : m_LineList)
	{
		const auto& pL = pLine->Get_Info();

		if ((m_pInfo->fX > pL.tLeft.fX) &&
			(m_pInfo->fX < pL.tRight.fX) &&
			(pL.tLeft.fX != pL.tRight.fX))
		{
			if (m_pInfo->fY <= pL.tfTilt * m_pInfo->fX + pL.tfYInter)
			{
				fDistance = sqrt(
					((pL.tfTilt * m_pInfo->fX - m_pInfo->fY +
						pL.tfYInter)
						* (pL.tfTilt * m_pInfo->fX - m_pInfo->fY +
							pL.tfYInter))
					/ ((pL.tfTilt * pL.tfTilt) + 1));

				if (fDistance < fTarget)
				{
					fTarget = fDistance;
					pTargetLine = pLine;
				}
			}
		}
	}

	if (!pTargetLine)
		return nullptr;
	else
	{
		if (fTarget <= m_pInfo->fCX / 2.f)
			return pTargetLine;
		else
			return nullptr;
	}


	return nullptr;
}

CLine* CLineMgr::Line_Movement_Rect(INFO* m_pInfo)
{
	CLine* pTargetLine = nullptr;

	float fDistance(0.f);
	float fTarget(INFINITY);

	if (m_LineList.empty())
		return nullptr;

	for (auto& pLine : m_LineList)
	{
		const auto& pL = pLine->Get_Info();

		if ((m_pInfo->fX > pL.tLeft.fX) &&
			(m_pInfo->fX < pL.tRight.fX) &&
			(pL.tLeft.fX != pL.tRight.fX))
		{
			if (m_pInfo->fY <= pL.tfTilt * m_pInfo->fX + pL.tfYInter)
			{
				fDistance = sqrt(
					((pL.tfTilt * m_pInfo->fX - m_pInfo->fY +
						pL.tfYInter)
						* (pL.tfTilt * m_pInfo->fX - m_pInfo->fY +
							pL.tfYInter))
					/ ((pL.tfTilt * pL.tfTilt) + 1));

				if (fDistance < fTarget)
				{
					fTarget = fDistance;
					pTargetLine = pLine;
				}
			}
		}
	}

	if (!pTargetLine)
		return nullptr;
	else
	{
		if (fabsf(pTargetLine->Get_Info().tfTilt) == 1.f)
		{
			if (fTarget <= m_pInfo->fCX * INV_SQRT2)
			{
				return pTargetLine;
			}
			else
				return nullptr;
		}
		else
		{
			if (fTarget <= m_pInfo->fCX * INV_SQRT2 * sinf(pTargetLine->Get_Info().tfRadian + (PI / 4.f)))
			{
				return pTargetLine;
			}
			else
				return nullptr;
		}
	}

	return nullptr;

}


CLine* CLineMgr::Collision_Vertical_Line(INFO* m_pInfo)
{
	CLine* pTargetLine = nullptr;

	float  fDistance(0.f);
	float  fTarget(INFINITY);
	float  fMinY(0.f);
	float  fMaxY(0.f);


	if (m_LineList.empty())
		return nullptr;

	for (auto& pLine : m_LineList)
	{
		fMinY = min(pLine->Get_Info().tRight.fY, pLine->Get_Info().tLeft.fY);
		fMaxY = max(pLine->Get_Info().tRight.fY, pLine->Get_Info().tLeft.fY);
		if ((m_pInfo->fY < fMaxY) &&
			(m_pInfo->fY > fMinY) &&
			(pLine->Get_Info().tLeft.fX == pLine->Get_Info().tRight.fX))
		{
			fDistance = fabsf(m_pInfo->fX - pLine->Get_Info().tRight.fX);

			if (fDistance < fTarget)
			{
				fTarget = fDistance;
				pTargetLine = pLine;
			}
		}
	}

	if (!pTargetLine)
		return nullptr;
	else
	{
		fDistance = fabsf(m_pInfo->fX - pTargetLine->Get_Info().tRight.fX);

		if (fDistance <= m_pInfo->fCX / 2.f)
			return pTargetLine;
		else
			return nullptr;
	}

	return nullptr;
}


void CLineMgr::Save_Line()
{
	HANDLE	hFile = CreateFile(L"../Data/LineCHA.dat", // 파일이름이 포함된 경로
		GENERIC_WRITE,	// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,			// 공유 방식(파일이 열려 있는 상태에서 다른 프로세스가 오픈하고자 할 때 허가할 것인가)
		NULL,			// 보안 모드 설정
		CREATE_ALWAYS,		// 생성 방식 CREATE_ALWAYS(쓰기 전용), OPEN_EXISTING(파일이 있는 경우만 열기)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등등), 아무런 속성이 없는 일반 파일ㄹ
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"File Open Failed", _T("FAIL"), MB_OK);
		return;
	}

	DWORD		dwByte(0);

	for (auto& pLine : m_LineList)
	{
		WriteFile(hFile, &(pLine->Get_Info()), sizeof(LINE), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Save Success", _T("SUCCESS"), MB_OK);

}

void CLineMgr::Load_Line()
{
	HANDLE	hFile = CreateFile(L"../Data/LineCHA.dat", // 파일이름이 포함된 경로
		GENERIC_READ,	// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,			// 공유 방식(파일이 열려 있는 상태에서 다른 프로세스가 오픈하고자 할 때 허가할 것인가)
		NULL,			// 보안 모드 설정
		OPEN_EXISTING,		// 생성 방식 CREATE_ALWAYS(쓰기 전용), OPEN_EXISTING(파일이 있는 경우만 열기)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등등), 아무런 속성이 없는 일반 파일
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"File Open Failed", _T("FAIL"), MB_OK);
		return;
	}

	DWORD		dwByte(0);
	LINE		tInfo{};

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(LINE), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_LineList.push_back(new CLine(tInfo.tLeft, tInfo.tRight));

	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Load Success", _T("SUCCESS"), MB_OK);
}

