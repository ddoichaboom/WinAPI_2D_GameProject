#pragma once

#include "CObj.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory() {}
	~CAbstractFactory() {}

public:
	static CObj* Create_Obj()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create_Obj(BUTTON_TYPE eButtonType)
	{
		CObj* pObj = new T;
		pObj->Set_Button_Type(eButtonType);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create_Obj(float fX, float fY, float fSpeed)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(fX, fY);

		return pObj;
	}

};

