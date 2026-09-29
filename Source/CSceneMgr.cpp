#include "pch.h"
#include "CSceneMgr.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() : m_pScene(nullptr),
m_eCurrentID(SC_LOGO), m_ePreID(SC_END)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID eScene)
{
	m_eCurrentID = eScene;

	if (m_ePreID != m_eCurrentID)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurrentID)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;

		case SC_LOBBY:
			m_pScene = new CLobby;
			break;

		case SC_EDIT:
			m_pScene = new CEdit;
			break;

		case SC_STAGE:
			m_pScene = new CStage;
			break;


		}

		m_pScene->Initialize();

		m_ePreID = m_eCurrentID;
	}

}

void CSceneMgr::Update(float fDeltaTime)
{
	m_pScene->Update(fDeltaTime);
}

void CSceneMgr::Late_Update(float fDeltaTime)
{
	m_pScene->Late_Update(fDeltaTime);
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}
