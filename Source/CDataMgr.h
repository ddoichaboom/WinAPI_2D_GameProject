#pragma once
#include "Define.h"

class CDataMgr
{
private:
	CDataMgr();
	~CDataMgr();

public:
	static CDataMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CDataMgr;

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
			delete m_pInstance;
		m_pInstance = nullptr;
	}

public:
	void	Initialize();
	void	Release();

	const	IMAGEDATA*	Get_ImageData(const wstring& wstrFrameKey) const;

private:
	void		Load_Object_Data();
	void		Insert_Image();

private:
	static CDataMgr* m_pInstance;

	unordered_map<wstring, IMAGEDATA*> m_mapImageData;
};

