#include "pch.h"
#include "CUi.h"

CUi::CUi()
{
	m_eRender = UI;
	m_fSpeed = 0.f;
	m_fVx = 0.f;
	m_fVy = 0.f;
	m_fJumpSpeed = 0.f;
	m_bOnGround = false;
	m_bPrevOnGround = false;
}

CUi::~CUi()
{

}


