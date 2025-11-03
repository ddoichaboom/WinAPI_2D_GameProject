#include "pch.h"
#include "CEdit.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"
#include "TcharIO.h"
#include "CDataMgr.h"
#include "CJelly.h"
#include "CObstacle.h"
#include "CChunkMgr.h"
#include "CItem.h"
#include "Factory.h"

CEdit::CEdit()
	:  m_iEditType(EDIT_FIRST_PLATFORM),
	m_bRenderVerticalLine(false), m_iJellyType(0), m_pFrameKey(nullptr),
	m_iObstacleType(0), m_iJellyTextType(0), m_iStageType(STAGE01),
	m_pStageFrameKey(nullptr), m_fStageWidth(0.f), m_fTileCX(0.f),
	m_iItemType(ITEM_ENERGY), m_iPrevStageType(ST_END),
	m_iChunkIndex(1), m_fChunkStartX(0.f)
{
	ZeroMemory(&m_tCursor, sizeof(POINT));
}

CEdit::~CEdit()
{
	Release();
}

void CEdit::Initialize()
{
	m_pStageFrameKey = L"STAGE01";
	m_pTileFrameKey = L"STAGE01_PLATFORM01";

	const IMAGEDATA* pStageData = CDataMgr::Get_Instance()->Get_ImageData(m_pStageFrameKey);

	if (pStageData)
	{
		m_fStageWidth = pStageData->tInfo.fCX;
		CScrollMgr::Get_Instance()->Set_StageWidth(m_fStageWidth);
	}

	const IMAGEDATA* pTileData = CDataMgr::Get_Instance()->Get_ImageData(m_pTileFrameKey);

	if (pTileData)
		m_fTileCX = pTileData->tInfo.fCX;
	

 }

void CEdit::Update(float fDeltaTime)
{
	Key_Input();

	switch (m_iStageType)
	{
	case STAGE01:
		m_pStageFrameKey = L"STAGE01";
		m_pTileFrameKey = L"STAGE01_PLATFORM01";
		break;

	case STAGE02:
		m_pStageFrameKey = L"STAGE02";
		m_pTileFrameKey = L"STAGE02_PLATFORM01";
		break;

	case STAGE03:
		m_pStageFrameKey = L"STAGE03";
		m_pTileFrameKey = L"STAGE03_PLATFORM01";
		break;
	}
	
	if (m_iPrevStageType != m_iStageType)
	{
		CTileMgr::Get_Instance()->Initialize(m_pStageFrameKey);

		m_iPrevStageType = m_iStageType;
	}


	const IMAGEDATA* pStageData = CDataMgr::Get_Instance()->Get_ImageData(m_pStageFrameKey);

	if (pStageData)
	{
		m_fStageWidth = pStageData->tInfo.fCX;
		CScrollMgr::Get_Instance()->Set_StageWidth(m_fStageWidth);
	}

	const IMAGEDATA* pTileData = CDataMgr::Get_Instance()->Get_ImageData(m_pTileFrameKey);

	if (pTileData)
		m_fTileCX = pTileData->tInfo.fCX;

	GetCursorPos(&m_tCursor);
	ScreenToClient(g_hWnd, &m_tCursor);

	m_tCursor.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	m_tCursor.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	
	m_pFrameKey = nullptr;



	switch (m_iEditType)
	{
	case EDIT_SECOND_PLATFORM:
	{
		if (m_iStageType == STAGE01)
			m_pFrameKey = L"STAGE01_PLATFORM02";
		else if (m_iStageType == STAGE02)
			m_pFrameKey = L"STAGE02_PLATFORM02";
		else if (m_iStageType == STAGE03)
			m_pFrameKey = L"STAGE03_PLATFORM02";
	}
		break;

	case EDIT_OBSTACLE:
		switch (m_iObstacleType)
		{
		case SMALL_OBSTACLE01:
		{
			if (m_iStageType == STAGE01)
				m_pFrameKey = L"STAGE01_OBSTACLE01";
			else if (m_iStageType == STAGE02)
				m_pFrameKey = L"STAGE02_OBSTACLE01";
			else if (m_iStageType == STAGE03)
				m_pFrameKey = L"STAGE03_OBSTACLE01";
		}
			break;

		case SMALL_OBSTACLE02:
		{
			if (m_iStageType == STAGE01)
				m_pFrameKey = L"STAGE01_OBSTACLE02";
			else if (m_iStageType == STAGE02)
				m_pFrameKey = L"STAGE02_OBSTACLE02";
			else if (m_iStageType == STAGE03)
				m_pFrameKey = L"STAGE03_OBSTACLE02";
		}
			break;

		case BIG_OBSTACLE01:
		{
			if (m_iStageType == STAGE01)
				m_pFrameKey = L"STAGE01_OBSTACLE03";
			else if (m_iStageType == STAGE02)
				m_pFrameKey = L"STAGE02_OBSTACLE03";
			else if (m_iStageType == STAGE03)
				m_pFrameKey = L"STAGE03_OBSTACLE03";
		}
			break;

		case BIG_OBSTACLE02:
		{
			if (m_iStageType == STAGE01)
				m_pFrameKey = L"STAGE01_OBSTACLE04";
			else if (m_iStageType == STAGE02)
				m_pFrameKey = L"STAGE02_OBSTACLE04";
			else if (m_iStageType == STAGE03)
				m_pFrameKey = L"STAGE03_OBSTACLE04";
		}
			break;

		case BIG_OBSTACLE03:
		{
			if (m_iStageType == STAGE01)
				m_pFrameKey = L"STAGE01_OBSTACLE05";
			else if (m_iStageType == STAGE02)
				m_pFrameKey = L"STAGE02_OBSTACLE05";
			else if (m_iStageType == STAGE03)
				m_pFrameKey = L"STAGE03_OBSTACLE05";
		}
			break;

		case BIG_OBSTACLE04:
		{
			if (m_iStageType == STAGE01)
				m_pFrameKey = L"STAGE01_OBSTACLE06";
			else if (m_iStageType == STAGE02)
				m_pFrameKey = L"STAGE02_OBSTACLE06";
			else if (m_iStageType == STAGE03)
				m_pFrameKey = L"STAGE03_OBSTACLE06";
		}
			break;

		}
		break;

	case EDIT_JELLY:
	{
		switch (m_iJellyType)
		{
		case JELLY_JELLYBEAN:
			m_pFrameKey = L"JELLY_JELLYBEAN";
			break;

		case JELLY_BEAR_PINK:
			m_pFrameKey = L"JELLY_BEAR_PINK";
			break;

		case JELLY_BEAR_YELLOW:
			m_pFrameKey = L"JELLY_BEAR_YELLOW";
			break;

		case JELLY_BEAR_BIG:
			m_pFrameKey = L"JELLY_BEAR_BIG";
			break;

		case JELLY_SILVERCOIN:
			m_pFrameKey = L"JELLY_COIN1";
			break;

		case JELLY_SILVERCOIN_BIG:
			m_pFrameKey = L"JELLY_COIN1_BIG";
			break;

		case JELLY_GOLDCOIN:
			m_pFrameKey = L"JELLY_COIN2";
			break;

		case JELLY_GOLDCOIN_BIG:
			m_pFrameKey = L"JELLY_COIN2_BIG";
			break;

		case JELLY_ALPHABET:
			switch (m_iJellyTextType)
			{
			case 0:
				m_pFrameKey = L"JELLY_ALPHABET_A";
				break;
			case 1:
				m_pFrameKey = L"JELLY_ALPHABET_B";
				break;
			case 2:
				m_pFrameKey = L"JELLY_ALPHABET_C";
				break;
			case 3:
				m_pFrameKey = L"JELLY_ALPHABET_D";
				break;
			case 4:
				m_pFrameKey = L"JELLY_ALPHABET_E";
				break;
			case 5:
				m_pFrameKey = L"JELLY_ALPHABET_F";
				break;
			case 6:
				m_pFrameKey = L"JELLY_ALPHABET_G";
				break;
			case 7:
				m_pFrameKey = L"JELLY_ALPHABET_H";
				break;
			case 8:
				m_pFrameKey = L"JELLY_ALPHABET_I";
				break;
			case 9:
				m_pFrameKey = L"JELLY_ALPHABET_J";
				break;
			case 10:
				m_pFrameKey = L"JELLY_ALPHABET_K";
				break;
			case 11:
				m_pFrameKey = L"JELLY_ALPHABET_L";
				break;
			case 12:
				m_pFrameKey = L"JELLY_ALPHABET_M";
				break;
			case 13:
				m_pFrameKey = L"JELLY_ALPHABET_N";
				break;
			case 14:
				m_pFrameKey = L"JELLY_ALPHABET_O";
				break;
			case 15:
				m_pFrameKey = L"JELLY_ALPHABET_P";
				break;
			case 16:
				m_pFrameKey = L"JELLY_ALPHABET_Q";
				break;
			case 17:
				m_pFrameKey = L"JELLY_ALPHABET_R";
				break;
			case 18:
				m_pFrameKey = L"JELLY_ALPHABET_S";
				break;
			case 19:
				m_pFrameKey = L"JELLY_ALPHABET_T";
				break;
			case 20:
				m_pFrameKey = L"JELLY_ALPHABET_U";
				break;
			case 21:
				m_pFrameKey = L"JELLY_ALPHABET_V";
				break;
			case 22:
				m_pFrameKey = L"JELLY_ALPHABET_W";
				break;
			case 23:
				m_pFrameKey = L"JELLY_ALPHABET_X";
				break;
			case 24:
				m_pFrameKey = L"JELLY_ALPHABET_Y";
				break;
			case 25:
				m_pFrameKey = L"JELLY_ALPHABET_Z";
				break;
			case 26:
				m_pFrameKey = L"JELLY_EXCLAMATION_MARK";
				break;
			}
			break;
		}
	}
	break;

	case EDIT_ITEM:
	{
		switch (m_iItemType)
		{
		case ITEM_ENERGY:
			m_pFrameKey = L"ITEM_ENERGY";
			break;

		case ITEM_ENERGY_BIG:
			m_pFrameKey = L"ITEM_ENERGY_BIG";
			break;

		case ITEM_BOOST:
			m_pFrameKey = L"ITEM_BOOST";
			break;

		case ITEM_BIGGER:
			m_pFrameKey = L"ITEM_GIANT";
			break;

		case ITEM_MAGNET:
			m_pFrameKey = L"ITEM_MAGNET";
			break;

		case ITEM_COIN:
			m_pFrameKey = L"ITEM_COIN";
			break;

		case ITEM_JELLY:
			m_pFrameKey = L"ITEM_JELLY";
			break;

		case ITEM_FEVER:
			m_pFrameKey = L"ITEM_FEVER";
			break;

		case ITEM_FEVER_BIG:
			m_pFrameKey = L"ITEM_FEVER_BIG";
			break;

		}
	}
		break;
	}
	

	CTileMgr::Get_Instance()->Update();

	
	CObjMgr::Get_Instance()->Update(fDeltaTime);

}

void CEdit::Late_Update(float fDeltaTime)
{
	
	CObjMgr::Get_Instance()->Late_Update(fDeltaTime);

		
	CTileMgr::Get_Instance()->Late_Update();

	 
	CScrollMgr::Get_Instance()->Scroll_Lock();

}

void CEdit::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC     hGroundDC = CBmpMgr::Get_Instance()->Find_Image(m_pStageFrameKey);

	BitBlt(hDC,
		iScrollX, iScrollY,
		m_fStageWidth, WINCY,
		hGroundDC,
		0, 0,
		SRCCOPY);

	CTileMgr::Get_Instance()->Render(hDC);


	// 수직선 생성
	if (m_bRenderVerticalLine)
	{
		for (int i = 0; i < (int)m_fStageWidth / m_fTileCX ; i++)
		{
			MoveToEx(hDC, i * m_fTileCX + iScrollX, 0, nullptr);
			LineTo(hDC, i * m_fTileCX + iScrollX, WINCY);
		}
	}

	// 마우스 위치에 두번째 플랫폼이 따라다니며 렌더링 되어야 함
	
		
	if (m_pFrameKey)
	{
		const IMAGEDATA* pImageData = CDataMgr::Get_Instance()->Get_ImageData(m_pFrameKey);

		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		if (hMemDC)
		{
			
			int iWidth = pImageData->tInfo.fCX;
			int iHeight = pImageData->tInfo.fCY;


			// 반투명 설정 - (투명도 50%)
			BLENDFUNCTION bf = { AC_SRC_OVER, 0, 128, AC_SRC_ALPHA };

			AlphaBlend(hDC,
				(int)m_tCursor.x + iScrollX - iWidth / 2, (int)m_tCursor.y + iScrollY - iHeight / 2,
				iWidth, iHeight,
				hMemDC,
				0, 0,
				iWidth, iHeight,
				bf);
		}


	}

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

	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		CScrollMgr::Get_Instance()->Set_ScrollX(10.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		CScrollMgr::Get_Instance()->Set_ScrollX(-10.f);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		bool bObjectExists = false;

		
		switch (m_iEditType)
		{
		case EDIT_FIRST_PLATFORM:
			CTileMgr::Get_Instance()->Picking(pt, true);
			break;

		case EDIT_SECOND_PLATFORM:
			for (auto& pTile : CTileMgr::Get_Instance()->Get_vecTile())
			{
				if (!pTile)
					break;

				if (PtInRect(pTile->Get_Rect(), pt))
				{
					if(static_cast<CTile*>(pTile)->Get_Tile_State())
					{
						bObjectExists = true;
						break;
					}
				}
			}

			if (!bObjectExists)
				CTileMgr::Get_Instance()->Add_Tile(pt.x, pt.y, m_pFrameKey);		
			break;

		case EDIT_OBSTACLE:
			for (auto& pObj : CObjMgr::Get_Instance()->Get_ObjList(OBSTACLE))
			{
				if (!pObj)
					break;

				if (!lstrcmp(pObj->Get_FrameKey(), m_pFrameKey))
				{
					if (PtInRect(pObj->Get_Rect(), pt))
					{
						bObjectExists = true;
						break;
					}
				}
			}

			if (!bObjectExists)
				CObjMgr::Get_Instance()->Add_Object(OBSTACLE,
					CAbstractFactory<CObstacle>::Create_Obj(
						pt.x, pt.y, CDataMgr::Get_Instance()->Get_ImageData(m_pFrameKey)));

			break;

		case EDIT_JELLY:
		{
			for (auto& pObj : CObjMgr::Get_Instance()->Get_ObjList(JELLY))
			{
				if (!pObj)
					break;

				if (!lstrcmp(pObj->Get_FrameKey(), m_pFrameKey))
				{
					if (PtInRect(pObj->Get_Rect(), pt))
					{
						bObjectExists = true;
						break;
					}
				}
			}

			if (!bObjectExists)
				CObjMgr::Get_Instance()->Add_Object(JELLY,
					CAbstractFactory<CJelly>::Create_Obj(
						pt.x, pt.y, CDataMgr::Get_Instance()->Get_ImageData(m_pFrameKey)));

			break;
		}
		case EDIT_ITEM:
		{
			for (auto& pObj : CObjMgr::Get_Instance()->Get_ObjList(ITEM))
			{
				if (!pObj)
					break;

				if (!lstrcmp(pObj->Get_FrameKey(), m_pFrameKey))
				{
					if (PtInRect(pObj->Get_Rect(), pt))
					{
						bObjectExists = true;
						break;
					}
				}
			}

			if (!bObjectExists)
			{
				const IMAGEDATA* pImageData = CDataMgr::Get_Instance()->Get_ImageData(m_pFrameKey);

				if (pImageData)
				{
					CObj* pNewItem = Create_Object_By_ID(ITEM, (float)pt.x, (float)pt.y, pImageData);
					if (pNewItem) 
					{
						CObjMgr::Get_Instance()->Add_Object(ITEM, pNewItem);
					}
				}
			}
			

			break;
		}
	}
		
}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		list<CObj*>* pObjList = nullptr;

		
		switch (m_iEditType)
		{
		case EDIT_FIRST_PLATFORM:
			CTileMgr::Get_Instance()->Picking(pt, false);
			break;

		case EDIT_SECOND_PLATFORM:
			CTileMgr::Get_Instance()->Delete_Tile(pt);
			break;

		case EDIT_OBSTACLE:
			pObjList = &CObjMgr::Get_Instance()->Get_ObjList(OBSTACLE);

			if (pObjList)
			{
				for (auto iter = pObjList->begin();
					iter != pObjList->end(); )
				{
					if (!lstrcmp((*iter)->Get_FrameKey(), m_pFrameKey))
					{
						if (PtInRect((*iter)->Get_Rect(), pt))
						{
							Safe_Delete(*iter);
							iter = pObjList->erase(iter);
							break;
						}
						else
							++iter;
					}
					else
						++iter;
				}
			}

			break;

		case EDIT_JELLY:
		{
			pObjList = &CObjMgr::Get_Instance()->Get_ObjList(JELLY);

			if (pObjList)
			{
				for (auto iter = pObjList->begin();
					iter != pObjList->end(); )
				{
					if (!lstrcmp((*iter)->Get_FrameKey(), m_pFrameKey))
					{
						if (PtInRect((*iter)->Get_Rect(), pt))
						{
							Safe_Delete(*iter);
							iter = pObjList->erase(iter);
							break;
						}
						else
							++iter;
					}
					else
						++iter;
				}
			}

			break;
		}
		case EDIT_ITEM:
		{
			pObjList = &CObjMgr::Get_Instance()->Get_ObjList(ITEM);

			if (pObjList)
			{
				for (auto iter = pObjList->begin();
					iter != pObjList->end(); )
				{
					if (!lstrcmp((*iter)->Get_FrameKey(), m_pFrameKey))
					{
						if (PtInRect((*iter)->Get_Rect(), pt))
						{
							Safe_Delete(*iter);
							iter = pObjList->erase(iter);
							break;
						}
						else
							++iter;
					}
					else
						++iter;
				}
			}

			break;
		}

	}
		
}

		
	if (CKeyMgr::Get_Instance()->Key_Down('M'))
	{
		if (m_iStageType < ST_END - 1)
			m_iStageType++;
		else
			m_iStageType = STAGE01;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		if (m_iStageType == STAGE01)
			Save_Chunk_Data(L"./Data/Stage01_Chunk_01.dat");
		else if (m_iStageType == STAGE02)
			Save_Chunk_Data(L"./Data/Stage02_Chunk_01.dat");
		else if (m_iStageType == STAGE03)
			Save_Chunk_Data(L"./Data/Stage03_Chunk_01.dat");
	}		


	//if (CKeyMgr::Get_Instance()->Key_Down('L'))
	//{
	//	if (m_iStageType == STAGE01)
	//		CChunkMgr::Get_Instance()->LoadChunk(L"./Data/Stage01_Chunk_01.dat");
	//	else if (m_iStageType == STAGE02)
	//		CChunkMgr::Get_Instance()->LoadChunk(L"./Data/Stage02_Chunk_01.dat");
	//	else if (m_iStageType == STAGE03)
	//		CChunkMgr::Get_Instance()->LoadChunk(L"./Data/Stage03_Chunk_01.dat");
	//}

	if (CKeyMgr::Get_Instance()->Key_Down('E'))
	{
		if (m_iEditType < EDIT_TYPE_END - 1)
			m_iEditType++;

		else
			m_iEditType = EDIT_FIRST_PLATFORM;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('I'))
	{
		if (m_iItemType < IT_END - 1)
			m_iItemType++;

		else
			m_iItemType = ITEM_ENERGY;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('R'))
	{
		m_bRenderVerticalLine = !m_bRenderVerticalLine;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('J'))
	{
		if (m_iJellyType < JT_END - 1)
			m_iJellyType++;
		else
			m_iJellyType = JELLY_JELLYBEAN;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('O'))
	{
		if (m_iObstacleType < OT_END - 1)
			m_iObstacleType++;
		else
			m_iObstacleType = SMALL_OBSTACLE01;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('T'))
	{
		if (m_iJellyTextType < 26)
			m_iJellyTextType++;
		else
			m_iJellyTextType = 0;
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

	RECT rcState = { 10, 10, 100, 50 };
	wchar_t szEditState[256] = L"";
	const wchar_t* VerticalLineMode = m_bRenderVerticalLine ? L"ON" : L"OFF";


	swprintf_s(szEditState, _countof(szEditState),
		L"수직선 (R) : %s  편집 단계 (E) : %d 장애물 타입 (O) : %d \n 젤리 타입 (J) :  %d 젤리 글자 타입 (T) : %d 아이템 타입 (I) : %d",
		VerticalLineMode, m_iEditType,m_iObstacleType, m_iJellyType, m_iJellyTextType, m_iItemType);
	DrawTextW(hDC, szEditState, -1, &rcState, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOCLIP);

	// (선택) 십자선 마커
	MoveToEx(hDC, screenPt.x - 5, screenPt.y, nullptr);
	LineTo(hDC, screenPt.x + 5, screenPt.y);
	MoveToEx(hDC, screenPt.x, screenPt.y - 5, nullptr);
	LineTo(hDC, screenPt.x, screenPt.y + 5);

	SetTextColor(hDC, oldColor);
	SetBkMode(hDC, oldBk);
}

void CEdit::Save_Chunk_Data(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath,
		GENERIC_WRITE,
		0,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"Chunk File Create Failed", L"Error", MB_OK);
		return;
	}

	CHUNK_META meta;
	meta.version = 1;
	meta.stageFrameKey = m_pStageFrameKey ? m_pStageFrameKey : L"STAGE01";
	meta.ichunkIndex = m_iChunkIndex;
	meta.fWorldStartX = m_fChunkStartX;

	Utils::WritePOD(hFile, meta.magic);
	Utils::WritePOD(hFile, meta.version);
	Utils::WritePOD(hFile, meta.ichunkIndex);
	Utils::WritePOD(hFile, meta.fWorldStartX);
	Utils::WriteTString(hFile, meta.stageFrameKey.c_str());

	const vector<CObj*>& vecTiles = CTileMgr::Get_Instance()->Get_vecTile();

	for (const auto& pTile : vecTiles)
	{
		if (static_cast<CTile*>(pTile)->Get_Tile_State())
		{
			OBJID eID = PLATFORM;
			Utils::WritePOD(hFile, eID);					// 1. 객체 ID 저장
			Utils::WritePOD(hFile, *pTile->Get_Info());		// 2. 객체 Info 구조체 저장
			Utils::WriteTString(hFile, pTile->Get_FrameKey());	// 3. 객체 FrameKey 저장
		}
	}

	const list<CObj*>& listObstacles = CObjMgr::Get_Instance()->Get_ObjList(OBSTACLE);

	for (const auto& pObj : listObstacles)
	{
		OBJID eID = OBSTACLE;
		Utils::WritePOD(hFile, eID);					// 1. 객체 ID 저장
		Utils::WritePOD(hFile, *pObj->Get_Info());		// 2. 객체 Info 구조체 저장
		Utils::WriteTString(hFile, pObj->Get_FrameKey());	// 3. 객체 FrameKey 저장
	}

	const list<CObj*>& listJellies = CObjMgr::Get_Instance()->Get_ObjList(JELLY);
	for (const auto& pObj : listJellies)
	{
		OBJID eID = JELLY;
		Utils::WritePOD(hFile, eID);					// 1. 객체 ID 저장
		Utils::WritePOD(hFile, *pObj->Get_Info());		// 2. 객체 Info 구조체 저장
		Utils::WriteTString(hFile, pObj->Get_FrameKey());	// 3. 객체 FrameKey 저장
	}

	const list<CObj*>& listItems = CObjMgr::Get_Instance()->Get_ObjList(ITEM);
	for (const auto& pObj : listItems)
	{
		OBJID eID = ITEM;
		Utils::WritePOD(hFile, eID);					// 1. 객체 ID 저장
		Utils::WritePOD(hFile, *pObj->Get_Info());		// 2. 객체 Info 구조체 저장
		Utils::WriteTString(hFile, pObj->Get_FrameKey());	// 3. 객체 FrameKey 저장
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Chunk Save Success!", L"Success", MB_OK);
}