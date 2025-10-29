#include "pch.h"
#include "CCollisionMgr.h"
#include "CTile.h"

void CCollisionMgr::Collision_Rect(list<CObj*> Dst, list<CObj*> Src)
{
	// 하강 중 상면에만 얹히는 규칙 + "가장 가까운 상면" 하나만 선택
	for (auto& pDst : Dst)
	{
		const float vy = pDst->Get_Y_Axis_Speed();
		if (vy < 0.f) {
			// 상승 중에는 상-하 보정 없음
			//pDst->Set_OnGround(false);
			continue;
		}

		// 후보 탐색(1패스)
		CObj* pBestSrc = nullptr;
		float   bestPenY = FLT_MAX;   // 최소 y침투량(fHeight)
		float   tmpW = 0.f, tmpH = 0.f;

		// 수평 겹침 허용 최소 폭(픽셀): 발 모서리에서 '찍' 걸리는 것 완화
		const float kMinOverlapX = 2.f;

		for (auto& pSrc : Src)
		{
			// 1) 수평 겹침 체크 (AABB 히트센터/폭 활용)
			const INFO* d = pDst->Get_Info();
			const INFO* s = pSrc->Get_Info();

			const float dstLeft = d->fHitX - d->fHitCX * 0.5f;
			const float dstRight = d->fHitX + d->fHitCX * 0.5f;
			const float srcLeft = s->fHitX - s->fHitCX * 0.5f;
			const float srcRight = s->fHitX + s->fHitCX * 0.5f;

			const float overlapX = min(dstRight, srcRight) - max(dstLeft, srcLeft);
			if (overlapX < kMinOverlapX)
				continue;

			// 2) Dst가 위, Src가 아래(상-하 후보만)
			if (d->fHitY >= s->fHitY)
				continue;

			// 3) AABB 침투량 계산
			float penW = 0.f, penH = 0.f;
			if (!Check_Rect(pDst, pSrc, &penW, &penH))
				continue;

			// 4) "세로 충돌"만(상·하) 채택
			if (penW <= penH)
				continue;

			// 5) 최소 y침투량을 갖는 상면 하나만 선택
			if (penH < bestPenY) {
				bestPenY = penH;
				pBestSrc = pSrc;
			}
		}

		// 보정(2패스): 하나만 처리
		if (pBestSrc)
		{
			// 즉시 y축 보정: "세로 침투량"만큼 위로 올림
			pDst->Set_PosY(-bestPenY);

			// 즉시 정지: 튐/관통 방지
			pDst->Set_Y_Axis_Speed(0.f);

			// 지면 상태 세팅
			pDst->Set_OnGround(true);

			// 히트중심 재정렬
			const INFO* d = pDst->Get_Info();
			pDst->Set_Hit_Pos(d->fX, d->fY + (d->fCY - d->fHitCY) * 0.5f);

			// 한 개만 처리하고 종료(다중 보정 금지)
			continue;
		}

		// 후보가 하나도 없으면 공중
		pDst->Set_OnGround(false);
	}
}

void CCollisionMgr::Collision_Rect(list<CObj*> Dst, vector<CObj*> Src)
{
	RECT	rc{};
	float	fVy(0.f);

	for (auto& Dst : Dst)
	{
		for (auto& Src : Src)
		{
			fVy = Dst->Get_Y_Axis_Speed();

			if ((Dst->Get_Info()->fHitY <= Src->Get_Info()->fHitY) && (fVy >= 0.f))
			{
				if (IntersectRect(&rc, Dst->Get_HitRect(), Src->Get_HitRect()))
				{
					Dst->Set_OnGround(true);
					Dst->Set_Y_Axis_Speed(0.f);
					Dst->Set_PosY(-(rc.bottom - rc.top));
					return;
				}
				else
					continue;
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
	float fHorizontal = abs(pDst->Get_Info()->fHitX - pSrc->Get_Info()->fHitX);
	float fVertical = abs(pDst->Get_Info()->fHitY - pSrc->Get_Info()->fHitY);

	float	fRadiusX = (pDst->Get_Info()->fHitCX + pSrc->Get_Info()->fHitCX) * 0.5f;
	float	fRadiusY = (pDst->Get_Info()->fHitCY + pSrc->Get_Info()->fHitCY) * 0.5f;

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
