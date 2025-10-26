#pragma once
#include "CObj.h"

class CButton 
	: public CObj
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

