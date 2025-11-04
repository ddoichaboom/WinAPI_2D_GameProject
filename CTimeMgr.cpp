#include "pch.h"
#include "CTimeMgr.h"
#include <profileapi.h>

CTimeMgr* CTimeMgr::m_pInstance = nullptr;

CTimeMgr::CTimeMgr()
	: m_liFrequency{}
	, m_fFixedDeltaTime(0.f)
	, m_fAccumulator(0.f)
	, m_fFrameRateCap(60.f)
	, m_fTargetDeltaTime(0.f)
	, m_liPrevCounter{}
	, m_fRealDeltaTime(0.f)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Initialize()
{
	// CPU의 클럭 수 ( 초당 진동 수 )
	QueryPerformanceFrequency(&m_liFrequency);

	// 고정된 업데이트 간격을 설정 
	// 이 값으로 모든 물리 업데이트가 동일한 시간 간격으로 수행된다.
	m_fFixedDeltaTime = 1.f / 60.f;
	m_fTargetDeltaTime = 1.f / m_fFrameRateCap;		// 목표 프레임 시간 계산
	QueryPerformanceCounter(&m_liPrevCounter); // 추가

}

void CTimeMgr::Update_DeltaTime()
{

	LARGE_INTEGER liCurCount;
	QueryPerformanceCounter(&liCurCount);

	// 이전 프레임과 현재 프레임의 클럭 수 차이를 계산한다.
	float fDeltaTime = float(liCurCount.QuadPart - m_liPrevCounter.QuadPart) / m_liFrequency.QuadPart;


	if (fDeltaTime > 0.25f)
		fDeltaTime = 0.25f;

	m_fRealDeltaTime = fDeltaTime;
	m_fAccumulator += fDeltaTime;
	m_liPrevCounter = liCurCount;

}

void CTimeMgr::WaitForFrame(LARGE_INTEGER liFrameStart)
{
	LARGE_INTEGER		liCurCount;

	float fFrameElapsedTime = 0.f;

	do {
		QueryPerformanceCounter(&liCurCount);

		fFrameElapsedTime = float(liCurCount.QuadPart - liFrameStart.QuadPart) /
			m_liFrequency.QuadPart;
	} while (fFrameElapsedTime < m_fTargetDeltaTime);

}


