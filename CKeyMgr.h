#pragma once

#include "Define.h"

class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();

public:
	bool		Key_Pressing(int iKey);
	bool		Key_Up(int iKey);	// 눌렀다가 뗏을 때
	bool		Key_Down(int iKey); // 누르자 마자

public:
	void		Update();


public:
	static CKeyMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CKeyMgr;
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
	static CKeyMgr* m_pInstance;
	bool			m_bKeyState[VK_MAX];
};

