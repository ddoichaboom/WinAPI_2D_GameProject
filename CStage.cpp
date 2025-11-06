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
#include "CEffectMgr.h"


CStage::CStage()
	: m_fWorldEndX(0.f), m_fBgTileWidth(0.f), 
	m_bPendingStageSwitch(false), m_iNextStageID(1)
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	Initialize_Stage_Config(1);

	m_fWorldEndX = 0.f;
	CScrollMgr::Get_Instance()->Set_StageWidth(100000.f);

	Load_Next_Chunk();

	float fPlayerPosX = 400.f;
	CObjMgr::Get_Instance()->Add_Object(PLAYER,
		CAbstractFactory<CPlayer>::Create_Obj(
			fPlayerPosX, 398.f,
			CDataMgr::Get_Instance()->Get_ImageData(L"Cookie0001")));

	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CButton>::Create_Obj(JUMP)));
	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CButton>::Create_Obj(SLIDE)));

	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CHpBar>::Create_Obj()));
	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CScore>::Create_Obj()));

	CUiMgr::Get_Instance()->Add_UI(static_cast<CUi*>(CAbstractFactory<CBonusTime>::Create_Obj()));

}

void CStage::Update(float fDeltaTime)
{
	// 스테이지 전환 대기 중이면 실행
	if (m_bPendingStageSwitch)
	{
		m_bPendingStageSwitch = false;
		Switch_Stage(m_iNextStageID);
		return;			// 이번 프레임은 전환만 처리 
	}

	CSoundMgr::Get_Instance()->Update();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
		CKeyMgr::g_bDebugRender = !CKeyMgr::g_bDebugRender;

	CObjMgr::Get_Instance()->Update(fDeltaTime);
	CUiMgr::Get_Instance()->Update(fDeltaTime);

	CEffectMgr::Get_Instance()->Update(fDeltaTime);

	CObjMgr::Get_Instance()->Check_Collision(PLAYER, PLATFORM, CObjMgr::RECT);
	CObjMgr::Get_Instance()->Check_Collision(PLAYER, JELLY, CObjMgr::COLLECT_JELLY);
	CObjMgr::Get_Instance()->Check_Collision(PLAYER, OBSTACLE, CObjMgr::OBSTACLE);
	CObjMgr::Get_Instance()->Check_Collision(PLAYER, ITEM, CObjMgr::COLLECT_ITEM);

	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Target(PLAYER);
	if (pPlayer)
	{
		const float px = pPlayer->Get_Info()->fX;
		const float preloadThreshold = m_fBgTileWidth * 0.9f;				 

		if (px > (m_fWorldEndX - preloadThreshold))
		{
			Load_Next_Chunk();
		}
	}

	
}

void CStage::Late_Update(float fDeltaTime)
{
	CUiMgr::Get_Instance()->Late_Update(fDeltaTime);

	CObjMgr::Get_Instance()->Late_Update(fDeltaTime);

	CEffectMgr::Get_Instance()->Late_Update(fDeltaTime);

	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CStage::Render(HDC hDC)
{
	int  iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int  iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	Render_Background_Tiled(hDC);

	CObjMgr::Get_Instance()->Render(hDC);

	CEffectMgr::Get_Instance()->Render(hDC);

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

	CEffectMgr::Get_Instance()->Release();
}

void CStage::Render_Background_Tiled(HDC hDC)
{
	const int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	const int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hBG = CBmpMgr::Get_Instance()->Find_Image(m_currentStage.bgTileKey.c_str());
	if (!hBG)
		return;

	const int camWorldX = -iScrollX;
	const int tileWidth = (int)m_fBgTileWidth;

	const int firstTileIdx = camWorldX / tileWidth;
	const int tilesNeeded = (WINCX / tileWidth) + 2;

	for (int i = 0; i < tilesNeeded; ++i)
	{
		const int tileIdx = firstTileIdx + i;
		const int tileWorldX = tileIdx * tileWidth;		
		const int tileScreenX = tileWorldX + iScrollX;	

		if ((tileScreenX + tileWidth < 0) || (tileScreenX > WINCX))
			continue;

		BitBlt(hDC,
			tileScreenX, iScrollY,
			tileWidth, WINCY,
			hBG,
			0, 0,
			SRCCOPY);
	}
}

void CStage::Initialize_Stage_Config(int stageID)
{
	m_currentStage.stageID = stageID;
	m_currentStage.currentChunkIndex = 0;

	if (stageID == 1)
	{
		m_currentStage.bgTileKey = L"STAGE01";
		m_currentStage.sndBgmPath = L"./Sound/STAGE01_BGM.mp3";
		m_currentStage.chunkPaths = {
			L"./Data/Stage01_Chunk_01.dat",
			L"./Data/Stage01_Chunk_02.dat",
			L"./Data/Stage01_Chunk_03.dat",
			L"./Data/Stage01_Chunk_04.dat",
			L"./Data/Stage01_Chunk_05.dat",
			L"./Data/Stage01_Chunk_06.dat",
			L"./Data/Stage01_Chunk_07.dat",
			L"./Data/Stage01_Chunk_08.dat",
			L"./Data/Stage01_Chunk_09.dat",
			L"./Data/Stage01_Chunk_10.dat",
		};
	}
	else if (stageID == 2)
	{
		m_currentStage.bgTileKey = L"STAGE02";
		m_currentStage.sndBgmPath = L"./Sound/STAGE02_BGM.mp3";
		m_currentStage.chunkPaths = {
			L"./Data/Stage02_Chunk_01.dat",
			L"./Data/Stage02_Chunk_02.dat",
			L"./Data/Stage02_Chunk_03.dat",
			L"./Data/Stage02_Chunk_04.dat",
			L"./Data/Stage02_Chunk_05.dat",
			L"./Data/Stage02_Chunk_06.dat",
			L"./Data/Stage02_Chunk_07.dat",
			L"./Data/Stage02_Chunk_08.dat",
			L"./Data/Stage02_Chunk_09.dat",
			L"./Data/Stage02_Chunk_10.dat",
		};
	}

	const IMAGEDATA* pTileData = CDataMgr::Get_Instance()->Get_ImageData(m_currentStage.bgTileKey);
	if (pTileData)
	{
		m_fBgTileWidth = pTileData->tInfo.fCX;
		m_currentStage.bgTileWidth = m_fBgTileWidth;
	}

	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Instance()->PlayBGM(m_currentStage.sndBgmPath.c_str(), 0.8f);
}

void CStage::Load_Next_Chunk()
{
	if (m_currentStage.currentChunkIndex >= m_currentStage.chunkPaths.size())
		return;

	wstring nextChunkPath = m_currentStage.chunkPaths[m_currentStage.currentChunkIndex];
	m_currentStage.currentChunkIndex++;

	LoadedChunkInfo info{};
	if (CChunkMgr::Get_Instance()->LoadChunk(nextChunkPath.c_str(), m_fWorldEndX, &info))
	{
		m_fWorldEndX += m_fBgTileWidth;
	}
}

void CStage::Switch_Stage(int nextStageID)
{
	CObjMgr::Get_Instance()->Delete_ID(PLATFORM);
	CObjMgr::Get_Instance()->Delete_ID(OBSTACLE);
	CObjMgr::Get_Instance()->Delete_ID(JELLY);
	CObjMgr::Get_Instance()->Delete_ID(ITEM);

	Initialize_Stage_Config(nextStageID);

	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Target(PLAYER);
	if (pPlayer)
	{
		pPlayer->Set_Pos(400.f, 398.f);
		pPlayer->Update_Rect(PLAYER);
	}
	CScrollMgr::Get_Instance()->Reset_Scroll();

	m_fWorldEndX = 0.f;

	Load_Next_Chunk();


}

void CStage::Request_Stage_Switch(int nextStageID)
{
	m_bPendingStageSwitch = true;
	m_iNextStageID = nextStageID;
}