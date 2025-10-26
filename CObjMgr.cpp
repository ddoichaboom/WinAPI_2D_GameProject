#include "pch.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"

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
		CCollisionMgr::Collision_RectEx(m_ObjList[Dst], m_ObjList[Src]);
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
			pObj->Render(hDC);
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
