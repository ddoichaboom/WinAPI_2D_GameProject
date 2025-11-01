#pragma once
#include "Define.h"

class CChunkMgr
{

private:
	CChunkMgr();
	~CChunkMgr();

public:
	void	Initialize(const list<wstring>& chunkList);
	int		Update();
	void	Release();
	void	LoadChunk(const TCHAR* pFilePath);

private:
	void	Load_Next_Chunk();
	void	Unload_Passed_Objects();

public:
	static CChunkMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CChunkMgr;
		}

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CChunkMgr* m_pInstance;

	list<wstring>		m_ChunkList;
	float				m_fMapTotalWidth;
	int					m_iCurrentChunkIndex;

};

