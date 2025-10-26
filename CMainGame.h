#pragma once

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render();
	void	Release();

private:
	HDC				m_hDC;

	TCHAR			m_szFPS[32];
	int				m_iFPS;
	DWORD			m_dwTime;
	ULONGLONG		m_currTime;			// Update 호출 시 시각 체크
	ULONGLONG		m_prevTime;			// 해당 프레임 생성시 시각 체크 
	float			m_deltaTime;		// 시간 차이 담는 변수

};

