#pragma once
#include "CAbstractFactory.h"
#include "CObj.h"
#include "CTile.h"
#include "CObstacle.h"
#include "CJelly.h"
#include "CEnergy.h"
#include "CBoost.h"
#include "CGiant.h"

inline CObj* Create_Object_By_ID(OBJID eID, float fX, float fY, const IMAGEDATA* pImageData)
{
    switch (eID)
    {
    case PLATFORM:
        return CAbstractFactory<CTile>::Create_Obj(fX, fY, pImageData);
    case OBSTACLE:
        return CAbstractFactory<CObstacle>::Create_Obj(fX, fY, pImageData);
    case JELLY:
        return CAbstractFactory<CJelly>::Create_Obj(fX, fY, pImageData);
    case ITEM:
    {
        const wstring& frameKey = pImageData->pFrameKey;
        if (frameKey.find(L"ENERGY") != wstring::npos)
        {
            return CAbstractFactory<CEnergy>::Create_Obj(fX, fY, pImageData);
        }
        else if (frameKey.find(L"BOOST") != wstring::npos)
        {
            return CAbstractFactory<CBoost>::Create_Obj(fX, fY, pImageData);
        }
        else if (frameKey.find(L"GIANT") != wstring::npos)
        {
            return CAbstractFactory<CGiant>::Create_Obj(fX, fY, pImageData);
        }
    }
        
    }
}