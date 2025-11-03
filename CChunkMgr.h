#pragma once
#include "Define.h"

struct LoadedChunkInfo
{
	CHUNK_META meta;
	float stageWidth = 0.f;
};

class CChunkMgr
{

private:
	CChunkMgr();
	~CChunkMgr();

public:
	bool	LoadChunk(const wchar_t* pFilePath, float offsetX, LoadedChunkInfo* outInfo);

	void	ClearSequence();
	void	Enqueue(const std::wstring& path);
	bool	LoadNext(float fWorldStartX, LoadedChunkInfo* outInfo);

private:
	bool LoadChunkInternal(HANDLE hFile, float offsetX, LoadedChunkInfo* outInfo);

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
	deque<std::wstring> m_queue;
	


};

