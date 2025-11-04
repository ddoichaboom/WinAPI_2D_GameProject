#include "pch.h"
#include "CUiMgr.h"

CUiMgr* CUiMgr::m_pInstance = nullptr;

CUiMgr::CUiMgr()
{

}

CUiMgr::~CUiMgr()
{
	Release();
}

void CUiMgr::Add_UI(CUi* pUi)
{
	if (pUi)
		m_listUi.push_back(pUi);
}

void CUiMgr::Update(float fDeltaTime)
{
	for (auto iter = m_listUi.begin(); iter != m_listUi.end(); )
	{
		int iResult = (*iter)->Update(fDeltaTime);

		if (iResult == OBJ_DEAD)
		{
			Safe_Delete(*iter);
			iter = m_listUi.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CUiMgr::Late_Update(float fDeltaTime)
{
	for (auto& pUi : m_listUi)
	{
		pUi->Late_Update(fDeltaTime);
	}
}

void CUiMgr::Render(HDC hDC)
{
	for (auto& pUi : m_listUi)
	{
		pUi->Render(hDC);
	}
}

void CUiMgr::Release()
{
	for_each(m_listUi.begin(), m_listUi.end(), Safe_Delete<CUi*>);
	m_listUi.clear();
}