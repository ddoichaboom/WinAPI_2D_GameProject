#pragma once
#include "CObj.h"

class CUi
	: public CObj
{
public:
	CUi();
	virtual ~CUi();


public:
	void	Initialize() override	PURE;

	int		Update(float deltaTime) override PURE;

	void	Late_Update(float deltaTime) override PURE;

	void	Render(HDC hDC) override PURE;

	void	Release() override PURE;

};

