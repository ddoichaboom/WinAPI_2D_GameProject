#include "pch.h"
#include "CChunkMgr.h"

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
