#pragma once

#include "Define.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void	Initialize()	PURE;
	virtual void	Update()		PURE;
	virtual void	Late_Update()	PURE;
	virtual void	Render(HDC hDC)	PURE;
	virtual void	Release()		PURE;

protected:
	ULONGLONG		m_currTime;			// Update 호출 시 시각 체크
	ULONGLONG		m_prevTime;			// 해당 프레임 생성시 시각 체크 
	float			m_deltaTime;		// 시간 차이 담는 변수

};

