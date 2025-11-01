#include "pch.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CScrollMgr.h"
#include "CPlayer.h"
#include "CKeyMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Check_Collision(OBJID Dst, OBJID Src, Method eMethod)
{
	switch (eMethod)
	{
	case CObjMgr::CIRCLE:
		CCollisionMgr::Collision_Circle(m_ObjList[Dst], m_ObjList[Src]);
		break;
	case CObjMgr::RECT:
		CCollisionMgr::Collision_Rect(m_ObjList[Dst], m_ObjList[Src]);
		break;
	case CObjMgr::COLLECT:
		CCollisionMgr::Collision_Collect(m_ObjList[Dst], m_ObjList[Src]);
		break;
	case CObjMgr::OBSTACLE:
		if (!m_ObjList[Dst].empty())
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_ObjList[Dst].front());

			if (pPlayer && (!pPlayer->Is_Invincible()))
			{
				if (CCollisionMgr::Collision_Obstacle(m_ObjList[Dst], m_ObjList[Src]))
				{
					pPlayer->On_Hit();
					pPlayer->Take_Damage(10.f);
				}
			}
		}
		break;
	case CObjMgr::LINE:
		break;
	}
}

CObj* CObjMgr::Get_Target(OBJID eID, CObj* pObj)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CObj* pTarget = nullptr;

	float	fDistance(0.f);

	for (auto& Dst : m_ObjList[eID])
	{
		if (Dst->Get_Dead())
			continue;

		float	fWidth = Dst->Get_Info()->fX - pObj->Get_Info()->fX;
		float	fHeight = Dst->Get_Info()->fY - pObj->Get_Info()->fY;

		float  fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || (fDistance > fDiagonal))
		{
			pTarget = Dst;
			fDistance = fDiagonal;
		}

	}

	return pTarget;
}

// 임시로 플레이어 찾아서 반환해주는 오버로드 타겟 체크 함수 
CObj* CObjMgr::Get_Target(OBJID eID)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CObj* pTarget = nullptr;

	float	fDistance(0.f);

	for (auto& Dst : m_ObjList[eID])
	{
		pTarget = Dst;
	}

	return pTarget;
}

void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	if (eID >= OBJ_END || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Update(float deltaTime)
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			if (Is_Culling(*iter))
			{
				++iter;
				continue;
			}
			int iResult = (*iter)->Update(deltaTime);

			if (iResult == OBJ_DEAD)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void CObjMgr::Late_Update(float deltaTime)
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			if (Is_Culling(pObj))
				continue;

			pObj->Late_Update(deltaTime);

			if (m_ObjList[i].empty())
				break;

			RENDERID eID = pObj->Get_RenderID();
			m_RenderList[eID].push_back(pObj);

		}
	}
}

void CObjMgr::Render(HDC hDC)
{
	for (UINT i = 0; i < RENDER_END; ++i)
	{
		m_RenderList[i].sort([](CObj* pDst, CObj* pSrc)->bool
			{
				return pDst->Get_Info()->fY < pSrc->Get_Info()->fY;
			});

		for (auto& pObj : m_RenderList[i])
		{
			if (Is_Culling(pObj))
				continue;
			pObj->Render(hDC);

			if (CKeyMgr::g_bDebugRender && pObj->Get_RenderID() != UI)
			{
				int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
				const auto* pRect = pObj->Get_Rect();
				HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
				HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

				HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

				Rectangle(hDC, pRect->left + iScrollX, pRect->top, pRect->right + iScrollX, pRect->bottom);

				SelectObject(hDC, hOldPen);
				SelectObject(hDC, hOldBrush);
				DeleteObject(hPen);
			}
		}

		m_RenderList[i].clear();
	}
}

void CObjMgr::Release()
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::Delete_ID(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}

bool CObjMgr::Is_Culling(CObj* pObj)
{
	if (pObj->Get_RenderID() == UI)
		return false;
	
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fBuffer = 400.f; // 화면 경계에서 약간의 여유 공간 

	float fScreenX = pObj->Get_Info()->fX + fScrollX;
	float fObjHalfSizeX = pObj->Get_Info()->fCX / 2.f;

	if ((fScreenX < -fObjHalfSizeX - fBuffer) || (fScreenX > WINCX + fObjHalfSizeX + fBuffer))
	{
		return true;		// 컬링 대상
	}

	return false;		// 컬링 대상이 아님 ( 화면에 보임 )
}