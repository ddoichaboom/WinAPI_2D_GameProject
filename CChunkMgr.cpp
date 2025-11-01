#include "pch.h"
#include "CChunkMgr.h"
#include "CObjMgr.h"
#include "CTileMgr.h"
#include "CDataMgr.h"
#include "TcharIO.h"

CChunkMgr* CChunkMgr::m_pInstance = nullptr;

CChunkMgr::CChunkMgr()
	: m_fMapTotalWidth(0.f), m_iCurrentChunkIndex(0)
{
}

CChunkMgr::~CChunkMgr()
{
	Release();
}

void	CChunkMgr::Initialize(const list<wstring>& chunkList)
{
}

int		CChunkMgr::Update()
{

	return 0;
}


void	CChunkMgr::Release()
{
	m_ChunkList.clear();
}

void CChunkMgr::Load_Next_Chunk()
{
}

void CChunkMgr::Unload_Passed_Objects()
{
}

void CChunkMgr::LoadChunk(const TCHAR* pFilePath)
{
	CObjMgr::Get_Instance()->Delete_ID(PLATFORM);
	CObjMgr::Get_Instance()->Delete_ID(OBSTACLE);
	CObjMgr::Get_Instance()->Delete_ID(JELLY);
	CObjMgr::Get_Instance()->Delete_ID(ITEM);
				// 타일 매니저의 모든 타일 삭제

	HANDLE hFile = CreateFile(
		pFilePath,
		GENERIC_READ, 0,
		nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		//MessageBox(g_hWnd, L"Chunk File Open Failed!", L"Error", MB_OK);
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

