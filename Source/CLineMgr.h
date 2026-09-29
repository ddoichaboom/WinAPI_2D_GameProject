#pragma once

#include "CLine.h"

class CLineMgr
{
public:
	enum LINEDIRECTION { LEFT, RIGHT, END };

private:
	CLineMgr();
	~CLineMgr();

public:
	void		Initialize();
	int			Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();


	bool		Collision_Line(float* pY, float fX);
	CLine* Line_Movement_Circle(INFO* m_pInfo);
	CLine* Line_Movement_Rect(INFO* m_pInfo);
	CLine* Collision_Vertical_Line(INFO* m_pInfo);


	void		Save_Line();
	void		Load_Line();


public:
	static CLineMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CLineMgr;
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
	static CLineMgr* m_pInstance;
	list<CLine*>	 m_LineList;

	LINEPOINT		m_tLinePoint[END];
	POINT			m_tCursor;
	bool			m_bLineStart;
	bool			m_bEditorMode;


};

