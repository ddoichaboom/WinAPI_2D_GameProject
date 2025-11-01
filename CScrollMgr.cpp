#include "pch.h"
#include "CScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr() 
	: m_fScrollX(0.f), m_fScrollY(0.f), m_fStageWidth(0.f)
{
}

CScrollMgr::~CScrollMgr()
{
}

// TODO : 현재 출력되고 있는 Stage 이미지가 뭔지에 따라  스크롤 락 조정 필요.
// 현재 Stage, Edit에서 스크롤락을 호출하고 있는데 그 때 매개변수로 전달받아서 
// CDataMgr::Get_Instance()->Get_ImageData()해서 fCX, fCY 로 스크롤락에 지정해주면 될듯?

void CScrollMgr::Scroll_Lock()
{
	if (0.f < m_fScrollX)
		m_fScrollX = 0.f;

	if (0.f < m_fScrollY)
		m_fScrollY = 0.f;


	if ((m_fStageWidth > 0.f) && (WINCX - m_fStageWidth > m_fScrollX))
		m_fScrollX = WINCX - m_fStageWidth;

	if (WINCY - 720 > m_fScrollY)
		m_fScrollY = WINCY - 720;

}
