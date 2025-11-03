#include "pch.h"
#include "CObj.h"

CObj::CObj()
	: m_fSpeed(0.f), m_bDead(false), m_bOnGround(true),
	m_fJumpSpeed(0.f), m_fVy(0.f), m_fVx(0.f), m_bPrevOnGround(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_tHitRect, sizeof(RECT));
}

CObj::~CObj()
{

}

void	CObj::Update_Rect(OBJID eID)
{
	m_tRect.left = long(m_tInfo.fX - (m_tInfo.fCX / 2.f));
	m_tRect.top = long(m_tInfo.fY - (m_tInfo.fCY / 2.f));
	m_tRect.right = long(m_tInfo.fX + (m_tInfo.fCX / 2.f));
	m_tRect.bottom = long(m_tInfo.fY + (m_tInfo.fCY / 2.f));

	// 히트박스 좌표 매핑
	if ((eID == PLAYER) || (eID == OBSTACLE))
		Set_Hit_Pos(m_tInfo.fX, m_tInfo.fY + (m_tInfo.fCY - m_tInfo.fHitCY) / 2.f);
	else
		Set_Hit_Pos(m_tInfo.fX, m_tInfo.fY);

	m_tHitRect.left = long(m_tInfo.fHitX - (m_tInfo.fHitCX / 2.f));
	m_tHitRect.top = long(m_tInfo.fHitY - (m_tInfo.fHitCY / 2.f));
	m_tHitRect.right = long(m_tInfo.fHitX + (m_tInfo.fHitCX / 2.f));
	m_tHitRect.bottom = long(m_tInfo.fHitY + (m_tInfo.fHitCY / 2.f));
}

void	CObj::Move_Frame(float deltaTime)
{
	// 상태 락 (초) 카운트 다운
	if (m_tFrame.stateLockRemainSec > 0.0f) {
		m_tFrame.stateLockRemainSec -= deltaTime;
		if (m_tFrame.stateLockRemainSec < 0.0f)
			m_tFrame.stateLockRemainSec = 0.0f;
	}

	// 애니메이션 프레임 진행 (초)
	m_tFrame.frameElapsedSec += deltaTime;

	// interval이 0이면 안전장치
	const float interval = (m_tFrame.frameIntervalSec > 0.0f) ? m_tFrame.frameIntervalSec : 0.001f;

	while (m_tFrame.frameElapsedSec >= interval)
	{
		m_tFrame.frameElapsedSec -= interval;

		// 프레임 전진
		++m_tFrame.iStart;

		if (m_tFrame.iStart > m_tFrame.iEnd) 
		{
			if (m_tFrame.bLoop)
			{
				m_tFrame.iStart = 0;
			}
			else 
			{
				m_tFrame.iStart = m_tFrame.iEnd;		// 1샷만 진행하고 마지막 프레임 유지
				break;
			}
		}
	}
}

