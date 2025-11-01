#pragma once

#include "CLogo.h"
#include "CLobby.h"
#include "CStage.h"
#include "CEdit.h"

class CSceneMgr
{
	
private:
	CSceneMgr();
	~CSceneMgr();

public:
	void	Scene_Change(SCENEID eScene);
	void	Update(float fDeltaTime);
	void	Late_Update(float fDeltaTime);
	void	Render(HDC hDC);
	void	Release();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSceneMgr;
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
	static CSceneMgr* m_pInstance;
	CScene* m_pScene;

	SCENEID			m_eCurrentID;
	SCENEID			m_ePreID;

};

