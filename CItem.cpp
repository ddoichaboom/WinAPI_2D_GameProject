#include "pch.h"
#include "CItem.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

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

	Update_Rect(ITEM);

	return OBJ_NOEVENT;
}

void CItem::Late_Update(float deltaTime)
{
	Move_Frame(deltaTime);

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


