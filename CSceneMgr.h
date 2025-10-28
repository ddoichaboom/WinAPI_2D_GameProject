#pragma once

#include "CLogo.h"
#include "CMenu.h"
#include "CStage01.h"
#include "CBossStage.h"
#include "CEdit.h"

class CSceneMgr
{
	
private:
	CSceneMgr();
	~CSceneMgr();

public:
	void	Scene_Change(SCENEID eScene);
	void	Update();
	void	Late_Update();
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

