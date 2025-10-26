#include "pch.h"
#include "CCollisionMgr.h"

void CCollisionMgr::Collision_Rect(list<CObj*> Dst, list<CObj*> Src)
{
	RECT	rc{};

	for (auto& Dst : Dst)
	{
		for (auto& Src : Src)
		{
			if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx(list<CObj*> Dst, list<CObj*> Src)
{
	float	fWidth(0.f), fHeight(0.f);

	for (auto& Dst : Dst)
	{
		for (auto& Src : Src)
		{
			if (Check_Rect(Dst, Src, &fWidth, &fHeight))
			{
				// 상 하 충돌
				if (fWidth > fHeight)
				{
					// 상 충돌
					if (Dst->Get_Info()->fY < Src->Get_Info()->fY)
					{
						Dst->Set_PosY(-fHeight);
					}

					// 하 충돌
					else
					{
						Dst->Set_PosY(fHeight);
					}
				}

				else // 좌우 충돌
				{
					// 좌 충돌
					if (Dst->Get_Info()->fX < Src->Get_Info()->fX)
					{
						Dst->Set_PosX(-fWidth);
					}

					// 우 충돌
					else
					{
						Dst->Set_PosX(fWidth);
					}
				}
			}
		}
	}
}

bool CCollisionMgr::Check_Rect(CObj* pDst, CObj* pSrc, float* pWidth, float* pHeight)
{
	float fHorizontal = abs(pDst->Get_Info()->fX - pSrc->Get_Info()->fX);
	float fVertical = abs(pDst->Get_Info()->fY - pSrc->Get_Info()->fY);

	float	fRadiusX = (pDst->Get_Info()->fCX + pSrc->Get_Info()->fCX) * 0.5f;
	float	fRadiusY = (pDst->Get_Info()->fCY + pSrc->Get_Info()->fCY) * 0.5f;

	if ((fRadiusX > fHorizontal) && (fRadiusY > fVertical))
	{
		*pWidth = fRadiusX - fHorizontal;
		*pHeight = fRadiusY - fVertical;
		return true;
	}

	return false;
}

void CCollisionMgr::Collision_Circle(list<CObj*> Dst, list<CObj*> Src)
{
	for (auto& Dst : Dst)
	{
		for (auto& Src : Src)
		{
			if (Check_Circle(Dst, Src))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Check_Circle(CObj* pDst, CObj* pSrc)
{
	float	fWidth = pDst->Get_Info()->fX - pSrc->Get_Info()->fX;
	float	fHeight = pDst->Get_Info()->fY - pSrc->Get_Info()->fY;

	float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	float	fRadius = (pDst->Get_Info()->fCX + pSrc->Get_Info()->fCX) * 0.5f;

	return fRadius >= fDiagonal;
}
