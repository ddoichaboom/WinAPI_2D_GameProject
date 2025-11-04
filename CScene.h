#pragma once

#include "Define.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void	Initialize()					PURE;
	virtual void	Update(float fDeltaTime)		PURE;
	virtual void	Late_Update(float fDeltaTime)	PURE;
	virtual void	Render(HDC hDC)					PURE;
	virtual void	Release()						PURE;



};

