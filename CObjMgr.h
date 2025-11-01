#pragma once

#include "CObj.h"

class CObjMgr
{
private:
	CObjMgr();
	CObjMgr(const CObjMgr& rhs) = delete;		// 삭제
	CObjMgr& operator=(CObjMgr& rObj) = delete;	// 삭제
	~CObjMgr();

public:
	enum Method { CIRCLE = 0, RECT, LINE, COLLECT, OBSTACLE };
	void	Check_Collision(OBJID Dst, OBJID Src, Method eMethod);


public:
	CObj*			Get_Target(OBJID eID, CObj* pObj);
	CObj*			Get_Target(OBJID eID);

	list<CObj*>&	Get_ObjList(OBJID eID) { return m_ObjList[eID]; };

public:
	void	Add_Object(OBJID eID, CObj* pObj);
	void	Update(float deltaTime = 0.f);
	void	Late_Update(float deltaTime = 0.f);
	void	Render(HDC hDC);
	void	Release();

	void	Delete_ID(OBJID eID);



private:
	list<CObj*>		m_ObjList[OBJ_END];
	list<CObj*>		m_RenderList[RENDER_END];
	bool			Is_Culling(CObj* pObj);		// 화면 컬링을 위한 함수 선언 추가

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CObjMgr;
		}

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private:
	static CObjMgr* m_pInstance;
};

