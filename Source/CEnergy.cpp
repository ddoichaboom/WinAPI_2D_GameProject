#include "pch.h"
#include "CEnergy.h"
#include "CObjMgr.h"
#include "CSceneMgr.h"
#include "CStage.h"

CEnergy::CEnergy()
	: m_eItemType(ITEM_ENERGY)
{
}

CEnergy::~CEnergy()
{
}


void    CEnergy::Initialize()
{
	if (wcsstr(m_pFrameKey.c_str(), L"_BIG"))
	{
		m_eItemType = ITEM_ENERGY_BIG;
	}
	else
	{
		m_eItemType = ITEM_ENERGY;
	}

	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.frameElapsedSec = 0.0f;
	m_tFrame.frameIntervalSec = 0.50f;
	m_tFrame.stateLockRemainSec = 0.00f;
	m_tFrame.bLoop = true;    // 루프

	m_eRender = GAMEOBJECT;
}

void CEnergy::Apply_Effect(CPlayer* pPlayer)
{
	if (!pPlayer)
		return;
	
	if (m_eItemType == ITEM_ENERGY)
	{
		pPlayer->Restore_Hp(20.f);
	}
	else if (m_eItemType == ITEM_ENERGY_BIG)
	{
		pPlayer->Restore_Hp(50.f);

		// 스테이지 전환 트리거 
		CScene* pScene = CSceneMgr::Get_Instance()->Get_Current_Scene();
		CStage* pStage = dynamic_cast<CStage*>(pScene);

		if (pStage)
		{
			if (wcsstr(pStage->Get_Current_Stage().bgTileKey.c_str(), L"STAGE01"))
			{
				pStage->Request_Stage_Switch(2);			// STAGE 2로 전환 예약
			}
			else if (wcsstr(pStage->Get_Current_Stage().bgTileKey.c_str(), L"STAGE02"))
			{
				pPlayer->Activate_Clear_Sequence(CLEAR_SEQUENCE_DURATION);
			}

		}
	}
}
