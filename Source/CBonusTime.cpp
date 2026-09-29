#include "pch.h"
#include "CBonusTime.h"
#include "CDataMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"

CBonusTime::CBonusTime()
	:m_pBgFrameKey(L"UI_BONUSTIME_BG"),
	m_pEffectFrameKey(L"UI_BONUSTIME_EFFECT"),
	m_pBgInfo(nullptr), m_pEffectInfo(nullptr),
	m_fEffectPosX(0.f), m_fEffectSpeed(200.f),
	m_pPlayer(nullptr)
{

}

CBonusTime::~CBonusTime()
{

}

void CBonusTime::Initialize()
{
	m_pBgInfo = CDataMgr::Get_Instance()->Get_ImageData(m_pBgFrameKey);
	m_pEffectInfo = CDataMgr::Get_Instance()->Get_ImageData(m_pEffectFrameKey);

	m_tInfo = m_pBgInfo->tInfo;

	m_tInfo.fX = 10.f;
	m_tInfo.fY = 35.f;

}

int CBonusTime::Update(float deltaTime)
{
	if (!m_pPlayer)
		m_pPlayer = CObjMgr::Get_Instance()->Get_Target(PLAYER);

	return OBJ_NOEVENT;
}

void CBonusTime::Late_Update(float deltaTime)
{
}

void CBonusTime::Render(HDC hDC)
{
	//HDC hBgDC = CBmpMgr::Get_Instance()->Find_Image(m_pBgFrameKey);

	//if (!hBgDC)
	//	return;

	//BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	//float fRenderX = m_tInfo.fX - (m_tInfo.fCX / 2.f);
	//float fRenderY = m_tInfo.fY - (m_tInfo.fCY / 2.f);

	//AlphaBlend(hDC,
	//	(int)m_tInfo.fX, (int)m_tInfo.fY,
	//	(int)m_tInfo.fCX, (int)m_tInfo.fCY,
	//	hBgDC,
	//	0, 0,
	//	(int)m_tInfo.fCX, (int)m_tInfo.fCY,
	//	bf);

	//if (m_pPlayer)
	//{
	//	CPlayer* pPlayer = static_cast<CPlayer*>(m_pPlayer);
	//	const map<wchar_t, bool>& alphabets = pPlayer->Get_BonusTime_Alphabets();

	//	//BONUSTIME 9글자
	//	const wchar_t bonusTimeText[] = L"BONUSTIME";
	//	const int letterCount = 9;

	//	// UI 크기 - 270 x 38, 한 글자당 30 x 38
	//	const float letterWidth		= 30.f;
	//	const float letterHeight	= 38.f;

	//	// 알파벳 이미지 크기 - 43 x 47
	//	const float alphabetImgWidth	= 43.f;
	//	const float alphabetImgHeight	= 47.f;

	//	// 각 칸의 시작 위치 
	//	float fStartX = m_tInfo.fX;
	//	float fStartY = m_tInfo.fY;

	//	for (int i = 0; i < letterCount; ++i)
	//	{
	//		wchar_t currentLetter = bonusTimeText[i];

	//		// 해당 알파벳을 수집했는지 확인
	//		if (alphabets.find(currentLetter) != alphabets.end())
	//		{
	//			// 알파벳 이미지 키 생성 (예: "JELLY_ALPHABET_B")
	//			wchar_t alphabetKey[32];
	//			swprintf_s(alphabetKey, L"JELLY_ALPHABET_%c", currentLetter);

	//			HDC hAlphabetDC = CBmpMgr::Get_Instance()->Find_Image(alphabetKey);
	//			if (hAlphabetDC)
	//			{
	//				// 칸의 중앙에 배치 (43x47 이미지를 30x38 칸에 맞춤)
	//				float renderX = fStartX + (i * letterWidth);
	//				float renderY = fStartY;

	//				// 알파벳 이미지 렌더링 (알파 블렌딩)
	//				AlphaBlend(hDC,
	//					(int)renderX, (int)renderY,
	//					(int)letterWidth, (int)letterHeight,
	//					hAlphabetDC,
	//					0, 0,
	//					(int)alphabetImgWidth, (int)alphabetImgHeight,
	//					bf);
	//			}
	//		}
	//	}
	//}
	HDC hBgDC = CBmpMgr::Get_Instance()->Find_Image(m_pBgFrameKey);

	if (!hBgDC)
		return;

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	// 배경 렌더링
	AlphaBlend(hDC,
		(int)m_tInfo.fX, (int)m_tInfo.fY,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hBgDC,
		0, 0,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,
		bf);

	// 수집된 알파벳 렌더링
	if (m_pPlayer)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pPlayer);
		const map<wchar_t, bool>& alphabets = pPlayer->Get_BonusTime_Alphabets();

		HDC hEffectDC = CBmpMgr::Get_Instance()->Find_Image(m_pEffectFrameKey);

		if (!hEffectDC)
			return;

		// BONUSTIME = 9글자
		const wchar_t bonusTimeText[] = L"BONUSTIME";
		const int letterCount = 9;

		// UI 크기: 270x38, 한 글자당 30x38 공간
		const int letterWidth = 30;
		const int letterHeight = 38;

		// 각 칸의 시작 위치
		int startX = (int)m_tInfo.fX;
		int startY = (int)m_tInfo.fY;

		for (int i = 0; i < letterCount; ++i)
		{
			wchar_t currentLetter = bonusTimeText[i];

			// 해당 알파벳을 수집했는지 확인
			if (alphabets.find(currentLetter) != alphabets.end())
			{
				// BONUSTIME_BG_EFFECT 이미지에서 해당 글자 위치 계산
				// 스프라이트 시트에서 i번째 글자의 srcX 위치
				int srcX = i * letterWidth;
				int srcY = 0;  

				// 화면에 렌더링할 위치
				int dstX = startX + (i * letterWidth);
				int dstY = startY;

				// BONUSTIME_BG_EFFECT에서 해당 부분만 잘라서 렌더링
				AlphaBlend(hDC,
					dstX, dstY,
					letterWidth, letterHeight,
					hEffectDC,
					srcX, srcY,
					letterWidth, letterHeight,
					bf);
			}
		}
	}
}

void CBonusTime::Release()
{

}