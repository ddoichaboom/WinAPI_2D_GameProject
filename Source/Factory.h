#pragma once
#include "CAbstractFactory.h"
#include "CObj.h"
#include "CTile.h"
#include "CObstacle.h"
#include "CJelly.h"
#include "CEnergy.h"
#include "CBoost.h"
#include "CGiant.h"
#include "CMagnet.h"
#include "CChangeJelly.h"
#include "CCoin.h"
#include "CEffect.h"

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
        else if (frameKey.find(L"MAGNET") != wstring::npos)
        {
            return CAbstractFactory<CMagnet>::Create_Obj(fX, fY, pImageData);
        }
        else if (frameKey.find(L"CHANGE_JELLY") != wstring::npos)
        {
            return CAbstractFactory<CChangeJelly>::Create_Obj(fX, fY, pImageData);
        }
        else if (frameKey.find(L"ITEM_COIN") != wstring::npos)
        {
            return CAbstractFactory<CCoin>::Create_Obj(fX, fY, pImageData);
        }
        // TODO fever item & fever_big

    }
        
    }
}

// 이펙트 생성 헬퍼 함수
inline CObj* Create_Effect(float fX, float fY, const wchar_t* effectKey)
{
    CEffect* pEffect = new CEffect;
    pEffect->Set_Pos(fX, fY);
    pEffect->Set_FrameKey(effectKey);
    pEffect->Initialize();
    return pEffect;
}