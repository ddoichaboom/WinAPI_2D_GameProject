#include "pch.h"
#include "CScene.h"

CScene::CScene()
	: m_currTime(GetTickCount64()), m_prevTime(m_currTime), m_deltaTime(0)
{
}

CScene::~CScene()
{
}
