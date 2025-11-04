#include "pch.h"
#include "CEnergy.h"
#include "CObjMgr.h"

CEnergy::CEnergy()
	: m_fHealAmount(0.f)
{
}

CEnergy::~CEnergy()
{
}


void    CEnergy::Initialize()
{
	if (wcsstr(m_pFrameKey.c_str(), L"_BIG"))
	{
		m_fHealAmount = 50.f;
	}
	else
	{
		m_fHealAmount = 20.f;
	}

	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.frameElapsedSec = 0.0f;
	m_tFrame.frameIntervalSec = 0.50f;
	m_tFrame.stateLockRemainSec = 0.00f;
	m_tFrame.bLoop = true;    // ·çÇÁ

	m_eRender = GAMEOBJECT;
}

void CEnergy::Apply_Effect(CPlayer* pPlayer)
{
	pPlayer->Restore_Hp(m_fHealAmount);
}
