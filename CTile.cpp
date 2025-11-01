#include "pch.h"
#include "CTile.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CTile::CTile()
    : m_bState(true)
{

}

CTile::~CTile()
{
    Release();
}

void CTile::Initialize()
{
    m_eRender = TILE;
}

int CTile::Update(float deltaTime)
{
    Update_Rect(PLATFORM);

    return 0;
}

void CTile::Late_Update(float deltaTime)
{

}

void CTile::Render(HDC hDC)
{
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    HDC     hTileDC = CBmpMgr::Get_Instance()->Find_Image(Get_FrameKey());

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    int srcX = 0;
    int srcY = 0;
    int srcW = (int)m_tInfo.fCX;
    int srcH = (int)m_tInfo.fCY;

    int dstX = (int)m_tRect.left + iScrollX;
    int dstY = (int)m_tRect.top + iScrollY;
    int dstW = (int)m_tInfo.fCX;
    int dstH = (int)m_tInfo.fCY;;

    AlphaBlend(hDC,
        dstX, dstY,
        dstW, dstH,
        hTileDC,
        srcX, srcY,
        srcW, srcH,
        bf);
}

void CTile::Release()
{
}
