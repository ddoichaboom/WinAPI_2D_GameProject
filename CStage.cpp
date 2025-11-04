#include "pch.h"
#include "CStage.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include <ctime>
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CButton.h"
#include "CTile.h"
#include "CTileMgr.h"
#include "CDataMgr.h"
#include "CChunkMgr.h"
#include "CUiMgr.h"
#include "CHpBar.h"
#include "CScore.h"
#include "Factory.h"
#include "CBonusTime.h"
#include "CSoundMgr.h"


CStage::CStage()
	:m_fStageWidth(0.f)
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"./Sound/STAGE01_BGM.mp3");

	CChunkMgr::Get_Instance()->ClearSequence();
	CChunkMgr::Get_Instance()->Enqueue(L"./Data/Stage01_Chunk_01.dat");
	CChunkMgr::Get_Instance()->Enqueue(L"./Data/Stage02_Chunk_01.dat");

	LoadedChunkInfo info{};
	const float firstStartX = 0.f;

	if (!CChunkMgr::Get_Instance()->LoadNext(firstStartX, &info))
		MessageBox(g_hWnd, L"First Chunk Load Failed", L"Error", MB_OK);

	m_stageKey = info.meta.stageFrameKey.empty() ? L"STAGE01" : info.meta.stageFrameKey;
	m_PrevstageKey = m_stageKey;
	m_iCurrChunkIndex = info.meta.ichunkIndex;

	// 첫 번째 세그먼트도 정수로 정렬
	m_fStageWidth = floorf((info.stageWidth > 0.f ? info.stageWidth : 0.f) + 0.5f);
	CScrollMgr::Get_Instance()->Set_StageWidth(m_fStageWidth);
	m_fWorldEndX = m_fStageWidth;

	m_segments.clear();
	if (!m_stageKey.empty() && m_fStageWidth > 0.f) {
		STAGE_SEGMENT s;
		s.startX = 0.f;
		s.width = m_fStageWidth;
		s.stageKey = m_stageKey;
		m_segments.push_back(std::move(s));
	}

	float fPlayerPosX = -CScrollMgr::Get_Instance()->Get_ScrollX() + 400.f;
	CObjMgr::Get_Instance()->Add_Object(PLAYER,
		CAbstractFactory<CPlayer>::Create_Obj(
			fPlayerPosX, 398.f,
			CDataMgr::Get_Instance()->Get_ImageData(L"Cookie0001")));

	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CButton>::Create_Obj(JUMP)));
	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CButton>::Create_Obj(SLIDE)));

	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CHpBar>::Create_Obj()));
	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CScore>::Create_Obj()));

	//CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CBonusTime>::Create_Obj()));

}

void CStage::Update(float fDeltaTime)
{
	if (m_PrevstageKey != m_stageKey)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);

		wstring strSoundKeyPath = L"./Sound/";
		strSoundKeyPath += m_stageKey;
		strSoundKeyPath += L"_BGM.mp3";

		CSoundMgr::Get_Instance()->PlayBGM(strSoundKeyPath.c_str(), 0.5f);

		m_PrevstageKey = m_stageKey;
	}

	CSoundMgr::Get_Instance()->Update();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
		CKeyMgr::g_bDebugRender = !CKeyMgr::g_bDebugRender;

	CObjMgr::Get_Instance()->Update(fDeltaTime);
	CUiMgr::Get_Instance()->Update(fDeltaTime);

	CObjMgr::Get_Instance()->Check_Collision(PLAYER, PLATFORM, CObjMgr::RECT);

	CObjMgr::Get_Instance()->Check_Collision(PLAYER, JELLY, CObjMgr::COLLECT_JELLY);

	CObjMgr::Get_Instance()->Check_Collision(PLAYER, OBSTACLE, CObjMgr::OBSTACLE);

	CObjMgr::Get_Instance()->Check_Collision(PLAYER, ITEM, CObjMgr::COLLECT_ITEM);


	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Target(PLAYER);

	if (pPlayer)
	{
		const float px = pPlayer->Get_Info()->fX;
		const float preloadThreshold = 300.f;		 

		if (px > (m_fWorldEndX - preloadThreshold))
		{
			LoadedChunkInfo next{};

			if (CChunkMgr::Get_Instance()->LoadNext(m_fWorldEndX, &next))
			{
				m_stageKey = next.meta.stageFrameKey.empty() ? m_stageKey : next.meta.stageFrameKey;

				const float nextStageWidth = next.stageWidth > 0.f ? next.stageWidth : m_fStageWidth;

				// float 오차 방지를 위해 정수로 반올림
				const float segStartX = floorf(m_fWorldEndX + 0.5f);
				const float segWidth = floorf(nextStageWidth + 0.5f);

				m_fWorldEndX = segStartX + segWidth;  // 정수 정렬된 값으로 누적

				CScrollMgr::Get_Instance()->Set_StageWidth((int)m_fWorldEndX);

				STAGE_SEGMENT s;
				s.startX = segStartX;
				s.width = segWidth;
				s.stageKey = m_stageKey;
				m_segments.push_back(move(s));


			}
		}
	}

	
}

void CStage::Late_Update(float fDeltaTime)
{
	CUiMgr::Get_Instance()->Late_Update(fDeltaTime);

	CObjMgr::Get_Instance()->Late_Update(fDeltaTime);

	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CStage::Render(HDC hDC)
{
	int  iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int  iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	Render_Backgrounds_Segmented(hDC);

	CObjMgr::Get_Instance()->Render(hDC);
	CUiMgr::Get_Instance()->Render(hDC);


	//if (CObjMgr::Get_Instance()->Get_Target(PLAYER))
	//{
	//	wchar_t szPlayer[64] = L"";
	//	swprintf_s(szPlayer, _countof(szPlayer),
	//		L"Player fX : %ld Player fY : %ld fVY : %ld   ",
	//		(long)CObjMgr::Get_Instance()->Get_Target(PLAYER)->Get_Info()->fX,
	//		(long)CObjMgr::Get_Instance()->Get_Target(PLAYER)->Get_Info()->fY,
	//		(long)CObjMgr::Get_Instance()->Get_Target(PLAYER)->Get_Y_Axis_Speed());

	//	RECT rcPlayerPos = { 10, 100, 100, 120 };
	//	DrawTextW(hDC, szPlayer, -1, &rcPlayerPos, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOCLIP);
	//}
}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(PLAYER);
}

void    CStage::Load_Chunk_Data(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(
		pFilePath,
		GENERIC_READ, 0,
		nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"Chunk File Open Failed!", L"Error", MB_OK);
		return;
	}

	while (true)
	{
		OBJID	eID; 
		INFO	tInfo;
		wstring strFrameKey;

		if (!Utils::ReadPOD(hFile, eID))
			break;
		if (!Utils::ReadPOD(hFile, tInfo))
			break;
		if (!Utils::ReadTString(hFile, strFrameKey))
			break;

		const IMAGEDATA* pImageData = CDataMgr::Get_Instance()->Get_ImageData(strFrameKey.c_str());

		if (!pImageData)
			continue;

		CObj* pObj = Create_Object_By_ID(eID, tInfo.fX, tInfo.fY, pImageData);

		if (pObj)
		{
			CObjMgr::Get_Instance()->Add_Object(eID, pObj);
		}
	}

	CloseHandle(hFile);
}

void CStage::Render_Backgrounds_Segmented(HDC hDC)
{
	const int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	const int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	// 카메라(화면) 위치는 정수 기준
	const int camL = -iScrollX;
	const int camR = camL + WINCX;

	for (const auto& seg : m_segments)
	{
		if (seg.width <= 0.f || seg.stageKey.empty())
			continue;

		// 세그먼트 경계는 이미 정수로 정렬되어 있음
		const int segL = (int)(seg.startX + 0.5f);
		const int segR = (int)(seg.startX + seg.width + 0.5f);

		// 겹치는 영역 계산 (정수 연산)
		const int overlapL = max(camL, segL);
		const int overlapR = min(camR, segR);
		if (overlapR <= overlapL)
			continue;

		const int drawW = overlapR - overlapL;
		const int destX = overlapL + iScrollX;  // 화면좌표
		const int destY = iScrollY;

		// 세그먼트 내부 소스 오프셋 (정수 연산)
		const int srcX = overlapL - segL;
		const int srcY = 0;

		HDC hBG = CBmpMgr::Get_Instance()->Find_Image(seg.stageKey.c_str());
		if (!hBG) continue;

		BitBlt(hDC, destX, destY, drawW, WINCY, hBG, srcX, srcY, SRCCOPY);
	}


}
