#pragma once
#include "CScene.h"

class CLogo : public CScene
{
public:
	CLogo();
	virtual ~CLogo();

public:
	void Initialize() override;
	void Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Render(HDC hDC) override;
	void Release() override;
};

