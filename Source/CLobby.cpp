#include "pch.h"
#include "CLobby.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CSoundMgr.h"
#include "CDataMgr.h"

CLobby::CLobby()
	: m_bCharacterHovered(false), m_iPrevMotion(-1)
{
	ZeroMemory(&m_ptStartBtn, sizeof(POINT));
	ZeroMemory(&m_szStartBtn, sizeof(SIZE));
	ZeroMemory(&m_rcStartBtn, sizeof(RECT));
	ZeroMemory(&m_ptCharacter, sizeof(POINT));
	ZeroMemory(&m_szCharacter, sizeof(SIZE));
	ZeroMemory(&m_rcCharacter, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CLobby::~CLobby()
{
	Release();
}

void CLobby::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"./Image/Lobby/Lobby.png", L"LOBBY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"./Image/Lobby/Ginger_Brave_Cookie.png", L"GINGER_BRAVE_COOKIE_LOBBY");

	CSoundMgr::Get_Instance()->PlayBGM(L"./Sound/Main_Lobby_BGM.mp3", 0.5f);

	// 스타트 버튼 초기화
	const IMAGEDATA* pInfo = CDataMgr::Get_Instance()->Get_ImageData(L"START_BUTTON");

	if (pInfo)
	{
		m_szStartBtn.cx = static_cast<LONG>(pInfo->tInfo.fCX);
		m_szStartBtn.cy = static_cast<LONG>(pInfo->tInfo.fCY);

		const int padX = 100;
		const int padY = 100;

		// 좌상단 좌표 = (화면폭-버튼폭-패딩, 화면높이-버튼높이-패딩)
		m_ptStartBtn.x = WINCX - m_szStartBtn.cx - padX;
		m_ptStartBtn.y = WINCY - m_szStartBtn.cy - padY;

		// 클릭 영역
		m_rcStartBtn = {
			m_ptStartBtn.x, m_ptStartBtn.y,
			m_ptStartBtn.x + m_szStartBtn.cx,
			m_ptStartBtn.y + m_szStartBtn.cy
		};
	}

	// 캐릭터 초기화
	const IMAGEDATA* pCharInfo = CDataMgr::Get_Instance()->Get_ImageData(L"GINGER_BRAVE_COOKIE_LOBBY");

	if (pCharInfo)
	{
		m_szCharacter.cx = static_cast<LONG>(pCharInfo->tInfo.fCX);
		m_szCharacter.cy = static_cast<LONG>(pCharInfo->tInfo.fCY);

		// 중심 좌표
		float fCenterX = WINCX / 2.f;
		float fCenterY = WINCY / 2.f;

		// 좌상단 좌표 = 중심 - 크기/2
		m_ptCharacter.x = static_cast<LONG>(fCenterX - m_szCharacter.cx / 2.f);
		m_ptCharacter.y = static_cast<LONG>(fCenterY - m_szCharacter.cy / 2.f);

		// 클릭 영역
		m_rcCharacter = {
			m_ptCharacter.x, m_ptCharacter.y,
			m_ptCharacter.x + m_szCharacter.cx,
			m_ptCharacter.y + m_szCharacter.cy
		};
	}

	// FRAME 초기화 (IDLE 상태)
	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 1;
	m_tFrame.iMotion = 0;  // IDLE
	m_tFrame.frameElapsedSec = 0.0f;
	m_tFrame.frameIntervalSec = 1.f;
	m_tFrame.stateLockRemainSec = 0.0f;
	m_tFrame.bLoop = true;
}

void CLobby::Update(float fDeltaTime)
{
	Key_Input();
	Character_Interaction();
	Move_Frame(fDeltaTime);
}

void CLobby::Late_Update(float fDeltaTime)
{

}

void CLobby::Render(HDC hDC)
{
	HDC     hLobbyDC = CBmpMgr::Get_Instance()->Find_Image(L"LOBBY");
	HDC		hBtn = CBmpMgr::Get_Instance()->Find_Image(L"START_BUTTON");
	HDC		hCharDC = CBmpMgr::Get_Instance()->Find_Image(L"GINGER_BRAVE_COOKIE_LOBBY");

	if (!hLobbyDC)
		return;

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	BLENDFUNCTION bf_StartBtn = { AC_SRC_OVER, 0, 205, AC_SRC_ALPHA };

	// 배경 렌더링
	AlphaBlend(hDC,
		0, 0,
		WINCX, WINCY,
		hLobbyDC,
		0, 0,
		WINCX, WINCY,
		bf);

	// 캐릭터 렌더링 (스프라이트 시트)
	if (hCharDC)
	{
		if (m_tFrame.iStart < 0) 
			m_tFrame.iStart = 0;

		int srcX = m_szCharacter.cx * m_tFrame.iStart;
		int srcY = m_szCharacter.cy * m_tFrame.iMotion;

		AlphaBlend(hDC,
			m_ptCharacter.x, m_ptCharacter.y,
			m_szCharacter.cx, m_szCharacter.cy,
			hCharDC,
			srcX, srcY,
			m_szCharacter.cx, m_szCharacter.cy,
			bf);
	}

	// 스타트 버튼 렌더링
	if (hBtn)
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		AlphaBlend(hDC,
			m_ptStartBtn.x, m_ptStartBtn.y,
			m_szStartBtn.cx, m_szStartBtn.cy,
			hBtn,
			0, 0,
			m_szStartBtn.cx, m_szStartBtn.cy,
			(PtInRect(&m_rcStartBtn, pt) ? bf_StartBtn : bf));
	}

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

	// --- 추가: 마우스 좌클릭으로 Start ---
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt); // 프로젝트 전역 hWnd 사용

		if (PtInRect(&m_rcStartBtn, pt))
		{
			CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE);
			return;
		}
	}
}

void CLobby::Character_Interaction()
{
	// 원-샷(비루프) 재생 중에는 상태 재진입/전환을 막아 깜빡임 방지
	if (m_tFrame.stateLockRemainSec > 0.0f ||
		(!m_tFrame.bLoop && m_tFrame.iStart != m_tFrame.iEnd))
		return;

	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	bool bHovered = PtInRect(&m_rcCharacter, pt);

	if (bHovered && !m_bCharacterHovered)
	{
		// 마우스 진입 → INTERACT(원샷)
		m_tFrame.iMotion = 1;                 // row 1
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 1;
		m_tFrame.frameElapsedSec = 0.0f;
		m_tFrame.frameIntervalSec = 0.5f;
		m_tFrame.bLoop = true;
		m_tFrame.stateLockRemainSec = 0.0f;
		m_iPrevMotion = 0;                    // from IDLE
	}
	else if (!bHovered && m_bCharacterHovered)
	{
		// 마우스 이탈 → RELEASE(원샷)
		m_tFrame.iMotion = 2;                 // row 2
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 1;
		m_tFrame.frameElapsedSec = 0.0f;
		m_tFrame.frameIntervalSec = 0.5f;
		m_tFrame.bLoop = false;
		m_tFrame.stateLockRemainSec = 0.0f;
		m_iPrevMotion = 1;                    // from INTERACT
	}

	m_bCharacterHovered = bHovered;
}


void CLobby::Move_Frame(float deltaTime)
{
	// 상태 락 카운트다운
	if (m_tFrame.stateLockRemainSec > 0.0f)
	{
		m_tFrame.stateLockRemainSec -= deltaTime;
		if (m_tFrame.stateLockRemainSec < 0.0f)
			m_tFrame.stateLockRemainSec = 0.0f;
	}

	// 원샷이 끝나 마지막 프레임에서 잠깐 멈췄다면 → IDLE로 자연 복귀
	if (!m_tFrame.bLoop && m_tFrame.stateLockRemainSec == 0.0f &&
		m_tFrame.iStart == m_tFrame.iEnd)
	{
		// 부드럽게 IDLE로 복귀
		m_tFrame.iMotion = 0;                 // IDLE (row 0)
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 1;
		m_tFrame.frameElapsedSec = 0.0f;
		m_tFrame.frameIntervalSec = 1.f;
		m_tFrame.bLoop = true;
		// 이후의 진행은 IDLE 기준으로 계속됨
	}

	// 프레임 진행
	m_tFrame.frameElapsedSec += deltaTime;
	const float interval = (m_tFrame.frameIntervalSec > 0.0f) ? m_tFrame.frameIntervalSec : 0.001f;

	while (m_tFrame.frameElapsedSec >= interval)
	{
		m_tFrame.frameElapsedSec -= interval;

		// 다음 프레임으로
		++m_tFrame.iStart;

		if (m_tFrame.iStart > m_tFrame.iEnd)
		{
			if (m_tFrame.bLoop)
			{
				// 루프 애니메이션은 자연 순환
				m_tFrame.iStart = 0;
			}
			else
			{
				// 원샷 애니: 마지막 프레임을 잠깐 유지(정지) → 깜빡임 방지
				m_tFrame.iStart = m_tFrame.iEnd;
				if (m_tFrame.stateLockRemainSec < 0.12f)    // 유지 시간(0.12~0.2 권장)
					m_tFrame.stateLockRemainSec = 0.12f;
				break; // 더 이상 진행하지 않고 유지
			}
		}
	}
}
