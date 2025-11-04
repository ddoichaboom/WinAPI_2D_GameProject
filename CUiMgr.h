#pragma once
#include "CUi.h"

class CUiMgr
{
private:
	CUiMgr();
	~CUiMgr();

public:
	static CUiMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CUiMgr;
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
	static CUiMgr*	m_pInstance;
	list<CUi*>		m_listUi;


public:
	void	Add_UI(CUi* pUi);
	void	Update(float fDeltaTime);
	void	Late_Update(float fDeltaTime);
	void	Render(HDC hDC);
	void	Release();
};

