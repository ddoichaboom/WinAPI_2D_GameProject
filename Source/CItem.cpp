#include "pch.h"
#include "CItem.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

CItem::CItem()
{
}

CItem::~CItem()
{
	Release();
}

int CItem::Update(float deltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pTarget = CObjMgr::Get_Instance()->Get_Target(PLAYER, this);

	if (m_pTarget)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pTarget);
		if (!pPlayer || !pPlayer->Is_Magnet_On())
		{
			m_pTarget = nullptr;
			m_fSpeed = 0.f;
			Update_Rect(ITEM);

			return OBJ_NOEVENT;
		}

		float fWidth = m_pTarget->Get_Info()->fHitX - m_tInfo.fHitX;
		float fHeight = m_pTarget->Get_Info()->fHitY - m_tInfo.fHitY;
		float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if (fDiagonal > pPlayer->Get_Magnet_Radius())
		{
			m_pTarget = nullptr;
			m_fSpeed = 0.f;
			Update_Rect(ITEM);

			return OBJ_NOEVENT;
		}

		m_fSpeed = 350.f;			// 임시로 200.f 작성 테스트 후 조정 

		float fRadian = acosf(fWidth / fDiagonal);
		m_fAngle = fRadian * 180.f / PI;
		if (m_tInfo.fHitY < m_pTarget->Get_Info()->fHitY)
			m_fAngle *= -1.f;

	}

	Update_Rect(ITEM);

	return OBJ_NOEVENT;
}

void CItem::Late_Update(float deltaTime)
{
	Move_Frame(deltaTime);

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f)) * deltaTime;
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f)) * deltaTime;

	Update_Rect(ITEM);

}

void CItem::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int srcX = (int)m_tInfo.fCX * m_tFrame.iStart;
	int srcY = (int)m_tInfo.fCY * m_tFrame.iMotion;
	int srcW = (int)m_tInfo.fCX;
	int srcH = (int)m_tInfo.fCY;

	int dstX = (int)m_tRect.left + iScrollX;
	int dstY = (int)m_tRect.top + iScrollY;
	int dstW = (int)m_tInfo.fCX;
	int dstH = (int)m_tInfo.fCY;

	HDC hItemDC = CBmpMgr::Get_Instance()->Find_Image(Get_FrameKey());

	if (!hItemDC)
		return;

	BLENDFUNCTION	bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	AlphaBlend(hDC,
		dstX, dstY,
		dstW, dstH,
		hItemDC,
		srcX, srcY,
		srcW, srcH,
		bf);
}

void CItem::Release()
{
}


