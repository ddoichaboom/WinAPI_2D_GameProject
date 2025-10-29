#include "pch.h"
#include "CTileMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"


CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{

		for (int i = 0; i < TILEX; ++i)
		{
			float fX = float((TILECX >> 1) + (TILECX * i));
			float fY = float(WINCY - (TILECY >> 1));

			CObj* pTile = CAbstractFactory<CTile>::Create_Obj(fX, fY, L"STAGE01_PLATFORM01");
			dynamic_cast<CTile*>(pTile)->Set_Tile_State(true);
			m_vecTile.push_back(pTile);
		}
}

int CTileMgr::Update()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fBuffer = 100.f;

	for (auto& pTile : m_vecTile)
	{
		float fScreenX = pTile->Get_Info()->fX + fScrollX;
		float fTileHalfSizeX = pTile->Get_Info()->fCX / 2.f;

		if ((fScreenX < -fTileHalfSizeX - fBuffer) || (fScreenX > WINCX + fTileHalfSizeX + fBuffer))
			continue;

		pTile->Update();
	}

	return 0;
}

void CTileMgr::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fBuffer = 100.f;

	for (auto& pTile : m_vecTile)
	{
		if (!pTile)
			continue;

		float fScreenX = pTile->Get_Info()->fX + fScrollX;
		float fTileHalfSizeX = pTile->Get_Info()->fCX / 2.f;

		if ((fScreenX < -fTileHalfSizeX - fBuffer) || (fScreenX > WINCX + fTileHalfSizeX + fBuffer))
			continue;

		if (dynamic_cast<CTile*>(pTile)->Get_Tile_State())
			pTile->Render(hDC);
	}

}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Picking(POINT pt, bool bState)
{
	float pfX(0.f);
	float pfY(0.f);
	float pfCX(0.f);
	float pfCY(0.f);

	for (auto& pTile : m_vecTile)
	{
		if (!pTile)
			continue;

		pfX = pTile->Get_Info()->fX;
		pfY = pTile->Get_Info()->fY;
		pfCX = pTile->Get_Info()->fCX;
		pfCY = pTile->Get_Info()->fCY;

		// 해당 타일 x 좌표 안에 마우스 좌표가 위치할 떄
		if ((pt.x >= pfX - pfCX / 2.f) && (pt.x <= pfX + pfCX / 2.f))
		{
			// 타일 y 범위 안에 마우스가 위치할 때
			if ((pt.y >= pfY - pfCY / 2.f) && (pt.y <= pfY + pfCY / 2.f))
			{
				dynamic_cast<CTile*>(pTile)->Set_Tile_State(bState);
			}
			else
				continue;
		}
	}

}

void CTileMgr::Add_Tile(float fX, float fY)
{
	CObj* pTile = CAbstractFactory<CTile>::Create_Obj(fX, fY, L"STAGE01_PLATFORM02");
	dynamic_cast<CTile*>(pTile)->Set_Tile_State(true);
	m_vecTile.push_back(pTile);

	pTile = nullptr;
}

void CTileMgr::Save_Tile(const TCHAR* pFilePath)
{
	HANDLE	hFile = CreateFile(pFilePath, // 파일이름이 포함된 경로
		GENERIC_WRITE,	// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,			// 공유 방식(파일이 열려 있는 상태에서 다른 프로세스가 오픈하고자 할 때 허가할 것인가)
		NULL,			// 보안 모드 설정
		CREATE_ALWAYS,		// 생성 방식 CREATE_ALWAYS(쓰기 전용), OPEN_EXISTING(파일이 있는 경우만 열기)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등등), 아무런 속성이 없는 일반 파일
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"File Open Failed", _T("FAIL"), MB_OK);
		return;
	}


	for (auto& pTile : m_vecTile)
	{
		if (!pTile) 
			continue;

		bool bTileState = static_cast<CTile*>(pTile)->Get_Tile_State();
		Utils::WritePOD(hFile, bTileState);
		Utils::WriteTString(hFile, pTile->Get_FrameKey());
		Utils::WritePOD(hFile, *pTile->Get_Info());
		
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Save Success", _T("SUCCESS"), MB_OK);
}

void CTileMgr::Load_Tile()
{
	HANDLE	hFile = CreateFile(L"./Data/Stage01_Tile.dat", // 파일이름이 포함된 경로
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




	Release();

	while (true)
	{
		bool bTileState{};
		if (!Utils::ReadPOD(hFile, bTileState)) break;  // EOF or error -> 종료
		std::basic_string<TCHAR> pFrameKey;
		if (!Utils::ReadTString(hFile, pFrameKey)) break;
		INFO tinfo{};
		if (!Utils::ReadPOD(hFile, tinfo)) break;

		if (bTileState)
		{
			CObj* pTile = CAbstractFactory<CTile>::Create_Obj(tinfo.fX, tinfo.fY, pFrameKey.c_str());
			static_cast<CTile*>(pTile)->Set_Tile_State(bTileState);
			m_vecTile.push_back(pTile);
		}
		else
			continue;

	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"Load Success", _T("SUCCESS"), MB_OK);
}
