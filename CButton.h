#pragma once
#include "CUi.h"

class CButton 
	: public CUi
{
public:
	CButton();
	virtual ~CButton();

public:
	void			Initialize() override;
	int				Update(float deltaTime) override;
	void			Late_Update(float deltaTime) override;
	void			Render(HDC hDC) override;
	void			Release() override;

private:
	void			Key_Input();
	int				m_iDrawID;


};

