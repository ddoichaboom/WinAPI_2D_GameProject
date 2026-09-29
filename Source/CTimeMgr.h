#pragma once

class CTimeMgr
{
private:
	CTimeMgr();
	~CTimeMgr();

public:
	static CTimeMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTimeMgr;

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
			delete m_pInstance;
		m_pInstance = nullptr;
	}

public:
	void	Initialize();
	void	Update_DeltaTime();
	void	WaitForFrame(LARGE_INTEGER liFrameStart);			// 프레임 제한을 위한 함수 추가 
	float	Get_DeltaTime() const { return m_fFixedDeltaTime; }
	float	Get_RealDeltaTime() const { return m_fRealDeltaTime; }
private:
	static CTimeMgr*	m_pInstance;

	LARGE_INTEGER		m_liFrequency;		// CPU 클럭 수
	LARGE_INTEGER		m_liPrevCounter;		// 이전 프레임의 클럭 수
	float				m_fRealDeltaTime;

	float				m_fFixedDeltaTime;		// 고정된 시간 간격 ( 물리 업데이트 용도 )
	float				m_fAccumulator;			// 실제 시간을 누적하는 변수 
	float				m_fFrameRateCap;		// 목표 프레임 (65.f)
	float				m_fTargetDeltaTime;		// 목표 프레임의 시간 간격


friend class CMainGame;
};

