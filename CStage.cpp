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
	CChunkMgr::Get_Instance()->ClearSequence();
	CChunkMgr::Get_Instance()->Enqueue(L"./Data/Stage01_Chunk_01.dat");
	CChunkMgr::Get_Instance()->Enqueue(L"./Data/Stage02_Chunk_01.dat");

	LoadedChunkInfo info{};
	const float firstStartX = 0.f;

	if (!CChunkMgr::Get_Instance()->LoadNext(firstStartX, &info))
		MessageBox(g_hWnd, L"First Chunk Load Failed", L"Error", MB_OK);

	m_stageKey = info.meta.stageFrameKey.empty() ? L"STAGE01" : info.meta.stageFrameKey;
	m_iCurrChunkIndex = info.meta.ichunkIndex;
	m_fStageWidth = info.stageWidth > 0.f ? info.stageWidth : 0.f;
	CScrollMgr::Get_Instance()->Set_StageWidth(m_fStageWidth);
	m_fWorldEndX = m_fStageWidth;

	// 플레이어 생성
	float fPlayerPosX = -CScrollMgr::Get_Instance()->Get_ScrollX() + 400.f;
	CObjMgr::Get_Instance()->Add_Object(PLAYER,
		CAbstractFactory<CPlayer>::Create_Obj(
			fPlayerPosX, 398.f,
			CDataMgr::Get_Instance()->Get_ImageData(L"GINGER_BRAVE_COOKIE")));

	// UI 버튼 생성
	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CButton>::Create_Obj(JUMP)));
	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CButton>::Create_Obj(SLIDE)));

	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CHpBar>::Create_Obj()));
	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CScore>::Create_Obj()));

	// 보너스타임 이미지 리소스 편집 후 재 구현 
	//CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CBonusTime>::Create_Obj()));

}

void CStage::Update(float fDeltaTime)
{
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
		const float preloadThreshold = 300.f;		// 끝에서 300px 남았을 때 미리 로드 

		if (px > (m_fWorldEndX - preloadThreshold))
		{
			LoadedChunkInfo next{};

			if (CChunkMgr::Get_Instance()->LoadNext(m_fWorldEndX, &next))
			{
				m_stageKey = next.meta.stageFrameKey.empty() ? m_stageKey : next.meta.stageFrameKey;

				const float nextStageWidth = next.stageWidth > 0.f ? next.stageWidth : m_fStageWidth;
				m_fWorldEndX += nextStageWidth;

				CScrollMgr::Get_Instance()->Set_StageWidth((int)m_fWorldEndX);
			}
		}
	}

	
}

void CStage::Late_Update(float fDeltaTime)
{
	CObjMgr::Get_Instance()->Late_Update(fDeltaTime);
	CUiMgr::Get_Instance()->Late_Update(fDeltaTime);


	CScrollMgr::Get_Instance()->Scroll_Lock();

}

void CStage::Render(HDC hDC)
{

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	HDC     hGroundDC = CBmpMgr::Get_Instance()->Find_Image(m_stageKey.c_str());

	BitBlt(hDC,
		iScrollX, iScrollY,
		(int)m_fStageWidth, WINCY,
		hGroundDC,
		0, 0,
		SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);
	CUiMgr::Get_Instance()->Render(hDC);

	// 디버깅 용 - 플레이어 좌표 추적  
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

