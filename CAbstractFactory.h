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

	static CObj* Create_Obj(float fX, float fY)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(fX, fY);
		pObj->Initialize();

		return pObj;
	}

	
	static CObj* Create_Obj(float fX, float fY, const TCHAR* pFrameKey)
	{
		CObj* pObj = new T;
		pObj->Set_FrameKey(pFrameKey);
		pObj->Set_Pos(fX, fY);
		pObj->Initialize();
		pObj->Set_Hit_Pos(fX, (fY + ((pObj->Get_Info()->fCY) - (pObj->Get_Info()->fHitCY)) / 2.f));

		return pObj;
	}

	static CObj* Create_Obj(float fX, float fY, const IMAGEDATA* pImageData)
	{
		CObj* pObj = new T;
		pObj->Set_FrameKey(pImageData->pFrameKey.c_str());
		pObj->Set_Info(pImageData->tInfo);
		pObj->Set_Pos(fX, fY);
		pObj->Initialize();
		pObj->Update_Rect(pImageData->eID);

		return pObj;
	}
};



