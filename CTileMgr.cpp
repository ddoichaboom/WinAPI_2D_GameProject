#include "pch.h"
#include "CTileMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CDataMgr.h"


CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	m_vecTile.reserve(300);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize(const TCHAR* pStageFrameKey)
{
	Release();

	wstring  platformKey = pStageFrameKey;
	platformKey += L"_PLATFORM01";

	auto pStageData = CDataMgr::Get_Instance()->Get_ImageData(pStageFrameKey);
	auto pTileData = CDataMgr::Get_Instance()->Get_ImageData(platformKey.c_str());

	if (!pStageData || !pTileData)
		return;

	int iTILECX = pTileData->tInfo.fCX;
	int iTILECY = pTileData->tInfo.fCY;
	int iTILECount = static_cast<int>(ceil(pStageData->tInfo.fCX / iTILECX));



	for (int i = 0; i < iTILECount; ++i)
	{
		float fX = float((iTILECX / 2) + (iTILECX * i));
		float fY = float(WINCY - (iTILECY  / 2));

		CObj* pTile = CAbstractFactory<CTile>::Create_Obj(fX, fY, CDataMgr::Get_Instance()->Get_ImageData(platformKey));

		if (pTile)
		{
			dynamic_cast<CTile*>(pTile)->Set_Tile_State(true);
			m_vecTile.push_back(pTile);
		}
		
	}
}

int CTileMgr::Update()
{
	// 화면 안 + 버퍼 앞부분에 있는 것들만 업데이트 하는 컬링 
	// 이후 ObjMgr에 적용 해야함.

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
	float fBuffer = 400.f;

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
	for (auto& pTile : m_vecTile)
	{
		if (!pTile)
			continue;

		if (PtInRect(pTile->Get_Rect(), pt))
		{
			dynamic_cast<CTile*>(pTile)->Set_Tile_State(bState);
			break;
		}
	}

}

void CTileMgr::Add_Tile(float fX, float fY, const TCHAR* pFrameKey)
{
	if (!pFrameKey)
		return;

	// 요청 - 매개변수로 프레임 키를 인자로 받아서 해당 플랫폼 프레임 키로 생성 후 저장
	CObj* pTile = CAbstractFactory<CTile>::Create_Obj(
		fX, fY, CDataMgr::Get_Instance()->Get_ImageData(pFrameKey));
	if (pTile)
	{
		dynamic_cast<CTile*>(pTile)->Set_Tile_State(true);
		m_vecTile.push_back(pTile);
	}
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
}


void	CTileMgr::Delete_Tile(POINT pt)
{
	for (auto iter = m_vecTile.begin(); iter != m_vecTile.end(); ++iter)
	{
		if (!(*iter))
			continue;

		if (PtInRect((*iter)->Get_Rect(), pt))
		{
			if (wcsstr((*iter)->Get_FrameKey(), L"PLATFORM02"))
			{
				Safe_Delete(*iter);
				m_vecTile.erase(iter);
				break;
			}
		}
	}
}

void	CTileMgr::Add_Tile_By_Obj(CTile* pTile)
{
	if (pTile)
	{
		m_vecTile.push_back(pTile);
	}
}