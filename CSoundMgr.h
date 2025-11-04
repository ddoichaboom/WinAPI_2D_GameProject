#pragma once

#include "pch.h"
#include "Define.h"


class CSoundMgr
{
private:
	CSoundMgr();
	~CSoundMgr();
	
public:

	static CSoundMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSoundMgr;
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void	Initialize();
	void	Release();
	void	Update();


	void	PlaySound(const wstring& strSoundKey, SOUND_CHANNEL eChannel, float fVolume= 1.f);
	void	PlayBGM(const wstring& strSoundKey, float fVolume = 1.f);
	void	StopSound(SOUND_CHANNEL eChannel);
	void	StopAll();
	void	SetChannelVolume(SOUND_CHANNEL eChannel, float fVolume );
	bool	Is_Playing(SOUND_CHANNEL eChannel);


private:
	void	LoadSound(const wstring& strSoundKey);
	string  WstringToString(const wstring& wstr);


private:
	static CSoundMgr* m_pInstance;

	FMOD::System* m_pSystem;								// FMOD의 핵심 시스템 객체
	FMOD::Channel* m_pChannelArr[MAX_CHANNEL];				// 채널 : 사운드가 재생되는 통로

	map<wstring, FMOD::Sound*> m_mapSound;		// 로드된 사운드들을 관리하는 맵 (Key : 파일 경로)
	
};