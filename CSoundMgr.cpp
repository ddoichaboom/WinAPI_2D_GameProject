#include "pch.h"
#include "CSoundMgr.h"
#include <locale>
#include <codecvt>
#include <fmod_errors.h>

CSoundMgr* CSoundMgr::m_pInstance = nullptr;

CSoundMgr::CSoundMgr()
	: m_pSystem(nullptr)
{
	for (int i = 0; i < MAX_CHANNEL; ++i)
	{
		m_pChannelArr[i] = nullptr;
	}
}

CSoundMgr::~CSoundMgr()
{
	Release();
}


void	CSoundMgr::Initialize()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(MAX_CHANNEL, FMOD_INIT_NORMAL, nullptr);
}

void	CSoundMgr::Release()
{
	for (auto& Pair : m_mapSound)
	{
		Pair.second->release();
		Pair.second = nullptr;
	}

	m_mapSound.clear();

	m_pSystem->close();
	m_pSystem->release();
}

void	CSoundMgr::Update()
{
	m_pSystem->update();
}

void CSoundMgr::PlaySound(const wstring& strSoundKey, SOUND_CHANNEL eChannel, float fVolume)
{
	if (m_pSystem == nullptr)
		return;

	if (eChannel >= MAX_CHANNEL)
		return;

	if (eChannel == SOUND_BGM) {
		OutputDebugString((L"PlaySound called for BGM: " + strSoundKey + L"\\n").c_str());
	}

	// 1. 사운드가 로드되어 있는지 확인하고, 없으면 로드 
	LoadSound(strSoundKey);

	auto iter = m_mapSound.find(strSoundKey);
	if (iter == m_mapSound.end())
		return;

	// 2. 사운드 재생
	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[eChannel]);

	// 3. 볼륨 설정
	if (m_pChannelArr[eChannel])
		m_pChannelArr[eChannel]->setVolume(fVolume);
}

void CSoundMgr::PlayBGM(const wstring& strSoundKey, float fVolume)
{
	PlaySound(strSoundKey, SOUND_BGM, fVolume);
}

void CSoundMgr::StopSound(SOUND_CHANNEL eChannel)
{
	if ((eChannel >= MAX_CHANNEL) || (m_pChannelArr[eChannel] == nullptr))
		return;

	if (eChannel == SOUND_BGM) {
		OutputDebugString(L"StopSound called for BGM\\n");
	}

	m_pChannelArr[eChannel]->stop();
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAX_CHANNEL; ++i)
	{
		if (m_pChannelArr[i] != nullptr)
			m_pChannelArr[i]->stop();
	}
}

void CSoundMgr::SetChannelVolume(SOUND_CHANNEL eChannel, float fVolume)
{
	if ((eChannel >= MAX_CHANNEL) || (m_pChannelArr[eChannel] == nullptr))
		return;

	m_pChannelArr[eChannel]->setVolume(fVolume);
}

void CSoundMgr::LoadSound(const wstring& strSoundKey)
{
	// 이미 로드된 사운드인지 확인
	if (m_mapSound.find(strSoundKey) != m_mapSound.end())
		return;

	string strPath = WstringToString(strSoundKey);
	FMOD::Sound* pSound = nullptr;

	// BGM 채널이면 루프, 아니면 기본 모드로 로드 
	FMOD_MODE eMode = (strSoundKey.find(L"BGM") != wstring::npos) ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;

	FMOD_RESULT result = m_pSystem->createSound(strPath.c_str(), eMode, nullptr, &pSound);

	if (result != FMOD_OK)
	{
		const char* errorString = FMOD_ErrorString(result);

		wchar_t wErrorMsg[256];
		size_t ConvertedChars = 0;
		mbstowcs_s(&ConvertedChars, wErrorMsg, errorString, 256);

		wstring errorDetails  = L"Sound Load Failed!\n\n";
		errorDetails += L"File: " + strSoundKey + L"\n";
		errorDetails += L"Error: " + wstring(wErrorMsg);

		MessageBox(g_hWnd, errorDetails.c_str(), L"FMOD Error", MB_OK);
		return;
		
	}

	m_mapSound.emplace(strSoundKey, pSound);
}

string CSoundMgr::WstringToString(const wstring& wstr)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(wstr);
}

bool	CSoundMgr::Is_Playing(SOUND_CHANNEL eChannel)
{
	if ((eChannel >= MAX_CHANNEL) || (m_pChannelArr[eChannel] == nullptr))
		return false;

	bool bIsPlaying = false;
	m_pChannelArr[eChannel]->isPlaying(&bIsPlaying);
	return bIsPlaying;
}
