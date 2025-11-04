#pragma once
#include "CScene.h"
class CLobby : public CScene
{
public:
	CLobby();
	virtual ~CLobby();

public:
	void Initialize() override;
	void Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	void	Key_Input();

};

