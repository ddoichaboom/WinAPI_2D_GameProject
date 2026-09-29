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


void CScrollMgr::Scroll_Lock()
{
	//if (0.f < m_fScrollX)
	//	m_fScrollX = 0.f;

	//if (0.f < m_fScrollY)
	//	m_fScrollY = 0.f;


	//if ((m_fStageWidth > 0.f) && (WINCX - m_fStageWidth > m_fScrollX))
	//	m_fScrollX = WINCX - m_fStageWidth;

	//if (WINCY - 720 > m_fScrollY)
	//	m_fScrollY = WINCY - 720;

    const float minX = WINCX - m_fStageWidth; // 오른쪽 끝(가장 왼쪽 스크롤 값)
    if (m_fScrollX > 0.f)
        m_fScrollX = 0.f;
    if (m_fScrollX < minX)
        m_fScrollX = minX;

    const float minY = WINCY - 720.f;
    if (m_fScrollY > 0.f)
        m_fScrollY = 0.f;
    if (m_fScrollY < minY)
        m_fScrollY = minY;

    // BitBlt는 정수 기준으로 픽셀을 복사하므로, 스크롤을 정수에 스냅하면
    // 세그먼트 경계 '헤어라인'이 사라짐
    m_fScrollX = floorf(m_fScrollX + 0.5f);
    m_fScrollY = floorf(m_fScrollY + 0.5f);

}

void CScrollMgr::FollowX(float targetX, float anchorX)
{
	m_fScrollX = -(targetX - anchorX);
}
