#include "pch.h"
#include "CBoost.h"

CBoost::CBoost()
{
}

CBoost::~CBoost()
{
}

void CBoost::Initialize()
{
	m_eRender = GAMEOBJECT;

	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.frameElapsedSec = 0.0f;
	m_tFrame.frameIntervalSec = 0.50f;
	m_tFrame.stateLockRemainSec = 0.00f;
	m_tFrame.bLoop = true;    // 루프
}

void CBoost::Apply_Effect(CPlayer* pPlayer)
{
	pPlayer->Set_Boost_Mode(2.0f);
}
