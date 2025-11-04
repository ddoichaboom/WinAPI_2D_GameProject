#include "pch.h"
#include "CChunkMgr.h"
#include "CObjMgr.h"
#include "CTileMgr.h"
#include "CDataMgr.h"
#include "Factory.h"
#include "TcharIO.h"

CChunkMgr* CChunkMgr::m_pInstance = nullptr;


CChunkMgr::CChunkMgr()
{
}

CChunkMgr::~CChunkMgr()
{
}



bool CChunkMgr::LoadChunk(const wchar_t* pFilePath, float offsetX, LoadedChunkInfo* outInfo)
{

	HANDLE hFile = CreateFile(
		pFilePath,
		GENERIC_READ, 0,
		nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	bool ok = LoadChunkInternal(hFile, offsetX, outInfo);
	CloseHandle(hFile);
	return ok;
}

bool CChunkMgr::LoadChunkInternal(HANDLE hFile, float offsetX, LoadedChunkInfo* outInfo)
{
	CHUNK_META meta{};
	if (!Utils::ReadPOD(hFile, meta.magic))
		return false; 
	if (meta.magic != 0x4B4E4843)
		return false;
	if (!Utils::ReadPOD(hFile, meta.version))
		return false;
	if (!Utils::ReadPOD(hFile, meta.ichunkIndex))
		return false;
	if (!Utils::ReadPOD(hFile, meta.fWorldStartX))
		return false;

	wstring stageKey;
	if (!Utils::ReadTString(hFile, stageKey))
		return false;

	meta.stageFrameKey = stageKey;

	float stageWidth = 0.f;
	if (const IMAGEDATA* pImageData = CDataMgr::Get_Instance()->Get_ImageData(stageKey)) 
	{
		stageWidth = pImageData->tInfo.fCX;
	}

	if (outInfo)
	{
		outInfo->meta = meta;
		outInfo->stageWidth = stageWidth;
	}

	while (true)
	{
		OBJID		eID;
		INFO		tInfo;
		wstring		frameKey;

		if (!Utils::ReadPOD(hFile, eID))
			break;
		if (!Utils::ReadPOD(hFile, tInfo))
			break;
		if (!Utils::ReadTString(hFile, frameKey))
			break;

		tInfo.fX += offsetX;

		const IMAGEDATA* pImageData = CDataMgr::Get_Instance()->Get_ImageData(frameKey);

		if (!pImageData)
			continue;

		CObj* pObj = Create_Object_By_ID(eID, tInfo.fX, tInfo.fY, pImageData);
		if (pObj)
			CObjMgr::Get_Instance()->Add_Object(eID, pObj);
	}

	return true;
}

void CChunkMgr::ClearSequence()
{
	m_queue.clear();
}

void CChunkMgr::Enqueue(const wstring& path)
{
	m_queue.push_back(path);
}

bool CChunkMgr::LoadNext(float WorldStartX, LoadedChunkInfo* outInfo)
{
	if (m_queue.empty())
		return false;

	wstring path = m_queue.front();
	m_queue.pop_front();

	return LoadChunk(path.c_str(), WorldStartX, outInfo);
}

